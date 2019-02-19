/*
 * 说明：Http请求子类（获取token）
 */
#ifndef HTTPGETACCESSTOKEN_H
#define HTTPGETACCESSTOKEN_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetAccessToken : public IHttpReqRes
{
public:
    explicit HttpGetAccessToken(QString sAppId, QString sAppSecret);
    ~HttpGetAccessToken();
protected:
    void processResponse(QString strResult);
    void processError();
};

#endif // HTTPGETACCESSTOKEN_H
