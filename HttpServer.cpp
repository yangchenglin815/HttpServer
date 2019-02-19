#include "HttpServer.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDomDocument>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "csha1.h"
#include "HttpAgent.h"
#include "constdata.h"
#include "unistd.h"
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "log4z.h"

#define TEST

const QString TOKEN = "yangcl";
#ifdef TEST
const QString APPID = "wx0775ae7b284f8c79";
const QString APPSECRET = "48f1cda51c1e47223eadc863244a4cc8";
#else
const QString APPID = "wx2b92ecc8e517bef2";
const QString APPSECRET = "4c3fafdbb34e84942dbc8cf0664c2c90";
#endif

using namespace std;


HttpServer::HttpServer(QObject *parent)
    : QObject(parent)
{
    m_tcpServer = new QTcpServer(this);
    m_tcpServer->setMaxPendingConnections(1);
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotOnNewConnection()));
    connect(HttpAgent::instance(), SIGNAL(sigOnGetAccessToken(int, QString, QString)), this, SLOT(slotOnGetAccessToken(int, QString, QString)));
    connect(HttpAgent::instance(), SIGNAL(sigOnCreateMenu(int, QString)), this, SLOT(slotOnCreateMenu(int, QString)));
}

HttpServer::~HttpServer()
{
    m_socket->close();
    m_tcpServer->close();
}

void HttpServer::run(quint16 port)
{
    QHostAddress address;
    address.setAddress("127.0.0.1");
    LOGI("start listen "<<address.toString().toStdString()<<":"<<port);
    if(!m_tcpServer->listen(address, port))
    {
        qWarning()<<m_tcpServer->errorString();
    }
    HttpAgent::instance()->RequestOnGetAccessToken(APPID, APPSECRET);
}

void HttpServer::StartLog(const char *logPath)
{
    zsummer::log4z::ILog4zManager::getInstance()->setLoggerMonthdir(LOG4Z_MAIN_LOGGER_ID, true);
    zsummer::log4z::ILog4zManager::getInstance()->setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, true);
    zsummer::log4z::ILog4zManager::getInstance()->setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_DEBUG);
    zsummer::log4z::ILog4zManager::getInstance()->setLoggerLimitsize(LOG4Z_MAIN_LOGGER_ID, 100);
    if (logPath) {
        zsummer::log4z::ILog4zManager::getInstance()->setLoggerPath(LOG4Z_MAIN_LOGGER_ID, logPath);
    }
    zsummer::log4z::ILog4zManager::getInstance()->start();
}

void HttpServer::slotOnNewConnection()
{
    m_socket = m_tcpServer->nextPendingConnection();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotOnRead()));
}

void HttpServer::slotOnRead()
{
    m_recv_msg += m_socket->readAll();


    if(m_recv_msg.startsWith("POST") && m_recv_msg.endsWith("</xml>")){
        protocolHandler(m_recv_msg);
    }
    else if(m_recv_msg.startsWith("GET") && m_recv_msg.endsWith("\r\n\r\n")){
        verifyToken(m_recv_msg);
    }
    else if(m_recv_msg.startsWith("POST") && m_recv_msg.endsWith("}")) {
        protocolTest(m_recv_msg);
    }
    else if(m_recv_msg.startsWith("POST") && m_recv_msg.endsWith("\r\n\r\n")){
        protocolTest(m_recv_msg);
    }
    else{
        return;
    }
    m_recv_msg.clear();
}

void HttpServer::slotOnGetAccessToken(int nCode, QString sMsg, QString sToken)
{
    LOGI("nCode: "<<nCode<<", sMsg: "<<sMsg.toStdString()<<", token: "<<sToken.toStdString());
    if(!sToken.isEmpty()) {
        m_token = sToken;

        QList<ButtonMenu_t> buttonList;
        ButtonMenu_t buttonMenu1, buttonMenu2, buttonMenu3;
        buttonMenu1.buttonMap.insert("name", "天气时报");
        buttonMenu1.buttonMap.insert("type", "view");
        buttonMenu1.buttonMap.insert("url", "http://www.weather.com.cn/weather/101250101.shtml");

        buttonMenu2.buttonMap.insert("name", "今日要闻");
        buttonMenu2.buttonMap.insert("type", "view");
        buttonMenu2.buttonMap.insert("url", "https://m.toutiao.com/");

        buttonMenu3.buttonMap.insert("name", "教务系统");
        ButtonMenu_t sub_button3, sub_button4, sub_button5;
        sub_button3.buttonMap.insert("name", "今日课表");
        sub_button3.buttonMap.insert("type", "view");
        sub_button3.buttonMap.insert("url", "https://yangchenglin815.github.io/timeTable/timeTable/src/login.html");
        buttonMenu3.subButtonList.append(sub_button3);
        sub_button4.buttonMap.insert("name", "明日课表");
        sub_button4.buttonMap.insert("type", "click");
        sub_button4.buttonMap.insert("key", "V1004");
        buttonMenu3.subButtonList.append(sub_button4);
        sub_button5.buttonMap.insert("name", "考试成绩");
        sub_button5.buttonMap.insert("type", "view");
        sub_button5.buttonMap.insert("url", "https://yangchenglin815.github.io/queryResult/login.html");
        buttonMenu3.subButtonList.append(sub_button5);
        buttonList.append(buttonMenu1);
        buttonList.append(buttonMenu2);
        buttonList.append(buttonMenu3);

        HttpAgent::instance()->RequestOnCreateMenu(buttonList, sToken);
    }
}

void HttpServer::slotOnCreateMenu(int nCode, QString sMsg)
{
    LOGI("HttpCreateMenu respond: "<<nCode<<"  "<<sMsg.toStdString());
}

QString HttpServer::getParameter(QString key, QByteArray dataArray)
{
    QString value;
    QString str = dataArray;
    QStringList list = str.split("\r\n");
    if(list.size() > 0) {
        QString dataLine = list.at(0);
        QString head = "GET /?";
        QString end = " HTTP/1.1";
        if (dataLine.startsWith(head) && dataLine.endsWith(end)) {
            dataLine.replace(head, "");
            dataLine.replace(end, "");
            QStringList list_ = dataLine.split("&");
            if(list_.size() > 1) {
                for(int i = 0; i < list_.size(); ++i){
                    if(list_[i].contains(key)) {
                        int index = list_[i].indexOf("=") + 1;
                        value = list_[i].right(list_[i].size() - index);
                    }
                }
            }
        }
    }
    return value;
}

void HttpServer::verifyToken(QByteArray array)
{
    QString signature = getParameter("signature", array);
    QString timestamp = getParameter("timestamp", array);
    QString nonce = getParameter("nonce", array); //随机数
    QString echostr = getParameter("echostr", array);//随机字符串

    QString sortStr = sort(TOKEN,timestamp,nonce);
    QString mySignature = QString::fromStdString(CSHA1::sha1(sortStr.toStdString()));
    if(!signature.isEmpty() && !mySignature.isEmpty() && mySignature == signature) {
        postData(echostr);
    }
}

void HttpServer::protocolHandler(QByteArray array)
{
    QString buf = array;
    QStringList list = buf.split("\r\n\r\n");
    if(list.size() > 1) {
        QString recvData = list.at(1);
        LOGI("recv xml: "<<recvData.toStdString());
        EventData_t eventData;
        eventData.MsgType = getElement("MsgType", recvData);
        eventData.ToUserName = getElement("ToUserName", recvData);
        eventData.FromUserName = getElement("FromUserName", recvData);
        eventData.Event = getElement("Event", recvData);
        if(eventData.MsgType == c_sEvent){
            LOGI("处理事件推送消息");
            EventHandler(eventData);
        }
        else if(eventData.MsgType == c_sMsg) {
            LOGI("处理消息回复");
            MsgHandler(eventData);
        }
    }
}

void HttpServer::protocolTest(QByteArray array)
{
    QString buf = array;
    QStringList list_ = buf.split("\r\n\r\n");
    QString recvData;
    if(list_.size() > 1) {
        recvData = list_.at(1);
    }

    QStringList list = buf.split("\r\n");
    if(list.size() > 1) {
        QString headData = list.at(0);
        list.clear();
        list = headData.split(" ");
        if(list.size() > 1){
            QString protocolHead = list.at(1);
            list.clear();
            list = protocolHead.split("/");
            if(list.size() > 1) {
                QString param = list.at(list.size() - 1);
                LOGI("reuqest param: "<<param.toStdString());
                if(param == "add") {
                    addInfo(recvData);
                }
                else if(param == "list"){
                    getListInfo();
                }
            }
        }
    }
}

QString HttpServer::sort(QString a, QString b, QString c)
{
    QVector<QString> var;
    var.push_back(a);
    var.push_back(b);
    var.push_back(c);

    QString data;
    qSort(var.begin(), var.end());
    for (int i = 0; i < var.size(); ++i) {
        data.append(var.at(i));
    }
    return data;
}

void HttpServer::postData(QString content)
{
    QString http = "HTTP/1.1 200 OK\r\n";
    http += "Server: nginx/1.14.0\r\n";
    http += "Access-Control-Allow-Origin:*\r\n";
    http += "Content-Type: application/octet-stream;charset=utf-8\r\n";
    http += "Connection: keep-alive\r\n";
    http += QString("Content-Length: %1\r\n\r\n").arg(content.length());
    http += content;

    if(m_socket != nullptr) {
        QByteArray headData;
        headData.append(http);
        LOGI("send data: "<<headData.data());
        m_socket->write(headData);
        m_socket->flush();
    }
}

QString HttpServer::getElement(QString tagName, QString xmlStream)
{
    QString value;
    QDomDocument doc;
    doc.setContent(xmlStream);
    QDomElement root=doc.documentElement(); //返回根节点
    QDomNode node=root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {

        QDomElement e = node.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if(e.tagName() == tagName){
                value = e.text();
                break;
            }
        }
        node=node.nextSibling();
    }
    return value;
}

void HttpServer::MsgHandler(EventData_t eventData)
{
    QString content = QStringLiteral("我是黑魔仙 小乐！！！");
    QString sendData =  QString("<xml>"
                                "<ToUserName><![CDATA[%1]]></ToUserName>"
                                "<FromUserName><![CDATA[%2]]></FromUserName>"
                                "<CreateTime>%3</CreateTime>"
                                "<MsgType><![CDATA[text]]></MsgType>"
                                "<Content><![CDATA[%4]]></Content>"
                                "</xml>").arg(eventData.FromUserName).arg(eventData.ToUserName).arg(QDateTime::currentDateTimeUtc().toSecsSinceEpoch()).arg(content);
    postData(sendData);
}

void HttpServer::ClickedEventHandler(EventData_t eventData)
{
    QString content = QStringLiteral("11月6日微语简报 农历九月二十九 星期二 工作愉快，生活喜乐！");
    QString sendData =  QString("<xml>"
                                "<ToUserName><![CDATA[%1]]></ToUserName>"
                                "<FromUserName><![CDATA[%2]]></FromUserName>"
                                "<CreateTime>%3</CreateTime>"
                                "<MsgType><![CDATA[text]]></MsgType>"
                                "<Content><![CDATA[%4]]></Content>"
                                "</xml>").arg(eventData.FromUserName).arg(eventData.ToUserName).arg(QDateTime::currentDateTimeUtc().toSecsSinceEpoch()).arg(content);
    postData(sendData);
}

void HttpServer::addInfo(QString bodyData)
{
    QJsonParseError json_error;
    QJsonDocument document(QJsonDocument::fromJson(bodyData.toUtf8(), &json_error));
    QJsonObject rootObj = document.object();
    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        qDebug() << "key" << i << " is:" << keys.at(i) << "value: " << rootObj[keys.at(i)].toString();
    }
    m_advertisementInfo.sDetial = rootObj["details"].toString();
    m_advertisementInfo.sImage = rootObj["logo"].toString();
    m_advertisementInfo.sTitle = rootObj["name"].toString();
    m_advertisementInfo.sUrl = rootObj["sponsorUrl"].toString();

    QJsonObject json;
    json.insert("code", 10001);
    json.insert("msg", "success");

    QJsonDocument recv_document;
    recv_document.setObject(json);
    QByteArray byte_array = recv_document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    postData(json_str);
}

void HttpServer::getListInfo()
{
    QJsonObject json1;
    json1.insert("indexImg", "");
    json1.insert("id", "");
    json1.insert("title", "");
    json1.insert("description", "");
    json1.insert("jumpUrl", "");
    json1.insert("delayTime", 5);

    QJsonArray array;
    array.insert(0, json1);
    QJsonObject object;
    object.insert("code", 200);
    object.insert("msg", "success");
    object.insert("datas", array);

    QJsonDocument recv_document;
    recv_document.setObject(object);
    QByteArray byte_array = recv_document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    postData(json_str);
}

void HttpServer::EventHandler(EventData_t eventData)
{
    if(eventData.Event == c_sSubscribe){
        LOGI("已订阅");
        QString content = QStringLiteral("欢迎来到魔法学校！！！");
        QString sendData =  QString("<xml>"
                                    "<ToUserName><![CDATA[%1]]></ToUserName>"
                                    "<FromUserName><![CDATA[%2]]></FromUserName>"
                                    "<CreateTime>%3</CreateTime>"
                                    "<MsgType><![CDATA[text]]></MsgType>"
                                    "<Content><![CDATA[%4]]></Content>"
                                    "</xml>").arg(eventData.FromUserName).arg(eventData.ToUserName).arg(QDateTime::currentDateTimeUtc().toSecsSinceEpoch()).arg(content);
        postData(sendData);

    }
    else if(eventData.Event == c_sUnsubscribe){
        LOGI("已取消订阅");
        postData("");
    }
    else if(eventData.Event == c_sClicked) {
        LOGI("点击菜单事件触发");
        ClickedEventHandler(eventData);
    }
}
