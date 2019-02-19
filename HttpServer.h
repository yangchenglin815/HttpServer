#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include "httpstructdata.h"

class QTcpServer;
class QTcpSocket;
class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer();

    void run(quint16 port);
    static void StartLog(const char* logPath = nullptr);
private slots:
    void slotOnNewConnection();
    void slotOnRead();
    void slotOnGetAccessToken(int nCode, QString sMsg, QString sToken);
    void slotOnCreateMenu(int nCode, QString sMsg);
private:
    QString getParameter(QString key, QByteArray dataArray);
    void verifyToken(QByteArray array);
    void protocolHandler(QByteArray array);
    void protocolTest(QByteArray array);
    QString sort(QString a, QString b, QString c);
    void postData(QString content);
    QString getElement(QString tagName, QString xmlStream);

    void EventHandler(EventData_t eventData);
    void MsgHandler(EventData_t eventData);
    void ClickedEventHandler(EventData_t eventData);

    void addInfo(QString bodyData);
    void getListInfo();
private:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_socket;
    QString m_token;
    QByteArray m_recv_msg;
    AdvertisementInfo m_advertisementInfo;
};

#endif // HTTPSERVER_H
