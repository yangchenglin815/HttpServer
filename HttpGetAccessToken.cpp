#include "HttpGetAccessToken.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumdata.h"
#include "constdata.h"
#include "HttpSignal.h"
#include "log4z.h"

HttpGetAccessToken::HttpGetAccessToken(QString sAppId, QString sAppSecret)
{
   QString allParam = QString("?grant_type=client_credential&appid=%1&secret=%2").arg(sAppId).arg(sAppSecret);
   setReqUrlAddress(c_sWinXinRequestUrl + c_sHttpGetAccessTokenAction + allParam);
}

HttpGetAccessToken::~HttpGetAccessToken()
{

}

void HttpGetAccessToken::processResponse(QString strResult)
{
   QScriptEngine engine;
   QScriptValue sc = engine.evaluate("value=" + strResult);
   int nCode = -1;
   QString sMsg;
   if(!sc.property("errcode").isValid()) {
      nCode = sc.property("errcode").toInt32();
      sMsg = sc.property("errmsg").toString();
   }

   if(sc.property("access_token").isValid()) {
      LOGI(this->getTestUrl().toStdString());
   }
   QString token = sc.property("access_token").toString();
   HttpSignal::instance()->sigOnGetAccessToken(nCode, sMsg, token);
   this->deleteLater();
}

void HttpGetAccessToken::processError()
{
   HttpSignal::instance()->sigOnGetAccessToken(e_connectServerError, c_sNetError, "");
   this->deleteLater();
}
