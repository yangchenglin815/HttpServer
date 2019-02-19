/*
 * 说明：http应答信号
 */
#ifndef HTTPSIGNAL_H
#define HTTPSIGNAL_H

#include <QObject>

class HttpSignal : public QObject
{
    Q_OBJECT
public:
    static HttpSignal* instance();
private:
    explicit HttpSignal(QObject *parent = nullptr);
    ~HttpSignal();
signals:
    void sigOnGetAccessToken(int nCode, QString sMsg, QString token);
    void sigOnCreateMenu(int nCode, QString sMsg);
private:
    static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
