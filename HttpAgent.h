/*
 * 说明：Http代理层(采用单例设计模式)
 */
#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>
#include "HttpSignal.h"
#include "httpstructdata.h"

class HttpAgent : public QObject
{
    Q_OBJECT
public:
    static HttpAgent* instance();

    void RequestOnGetAccessToken(QString sAppId, QString sAppSecret);
    void RequestOnCreateMenu(QList<ButtonMenu_t> buttonList, QString accessToken);
private:
    explicit HttpAgent(QObject *parent = nullptr);
    ~HttpAgent();

    void initConnect();
signals:
    void sigOnGetAccessToken(int nCode, QString sMsg, QString sToken);
    void sigOnCreateMenu(int nCode, QString sMsg);
private:
   static HttpAgent* m_pInstance;
};

#endif // HTTPAGENT_H
