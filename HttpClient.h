/*
 *  说明：Http请求层（采用单例设计模式）
 *  业务层---->代理层---->请求层---->通过对应请求的子类发起请求
 */
#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "httpstructdata.h"

class HttpClient : public QObject
{
    Q_OBJECT
public:
    static HttpClient* instance();
    void setToken(QString sToken);

    void RequestOnGetAccessToken(QString sAppId, QString sAppSecret);
    void RequestOnCreateMenu(QList<ButtonMenu_t> buttonList, QString accessToken);
private:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient();
private:
    static HttpClient* m_pInstance;
    QString m_strTokenInfo;
    QNetworkAccessManager* m_pManager;
    QString m_sToken;
};

#endif // HTTPCLIENT_H
