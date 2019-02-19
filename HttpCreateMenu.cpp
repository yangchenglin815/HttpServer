#include "HttpCreateMenu.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumdata.h"
#include "constdata.h"
#include "HttpSignal.h"
#include "log4z.h"

HttpCreateMenu::HttpCreateMenu(QList<ButtonMenu_t> buttonList, QString accessToken)
{
    setReqUrlAddress(c_SHttpCreateMenuAction + accessToken);
    QString allParam = createButtonJson(buttonList);
    QByteArray array;
    array.append(allParam);
    setReqParams(array);
}

void HttpCreateMenu::processResponse(QString strResult)
{
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("value=" + strResult);

    int nCode = sc.property("errcode").toInt32();
    QString sMsg = sc.property("errmsg").toString();
    if(!sc.property("errcode").isValid()) {
        nCode = e_connectServerError;
        sMsg = c_sServerJsonError;
        LOGI("HttpCreateMenu 返回参数json格式错误");
    }

    if(nCode != e_success) {
        LOGI(this->getTestUrl().toStdString());
    }

    HttpSignal::instance()->sigOnCreateMenu(nCode, sMsg);
    this->deleteLater();
}

void HttpCreateMenu::processError()
{
    HttpSignal::instance()->sigOnCreateMenu(e_connectServerError, c_sNetError);
    this->deleteLater();
}

QString HttpCreateMenu::createButtonJson(QList<ButtonMenu_t> buttonList)
{
    QJsonArray jsonArray;
    QJsonObject jsonObject;

    int nLen = buttonList.length();
    ButtonMenu_t buttonInfo;
    for (int i = 0; i < nLen; i++)
    {
        buttonInfo = buttonList[i];
        QJsonObject dataJson;
        QMapIterator<QString, QString> it(buttonInfo.buttonMap);
        while(it.hasNext()) {
            it.next();
            dataJson.insert(it.key(), it.value());
        }

        if(buttonInfo.subButtonList.size() > 0) {
            QJsonArray array;
            for(int j = 0; j < buttonInfo.subButtonList.size(); ++j) {
               ButtonMenu_t menu = buttonInfo.subButtonList[j];
               QJsonObject object;
               QMapIterator<QString, QString> it(menu.buttonMap);
               while (it.hasNext()) {
                   it.next();
                   object.insert(it.key(), it.value());
               }
               array.insert(j, object);
            }
            dataJson.insert("sub_button", array);
        }
        jsonArray.insert(i, dataJson); //
    }
     jsonObject.insert("button", jsonArray);

    QJsonDocument document;
    document.setObject(jsonObject);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    return QString(byte_array);
}
