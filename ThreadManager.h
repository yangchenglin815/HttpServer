/*
 * 说明：工作线程类
 */
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    static QThread* getAgentThread();
private:
    static QThread* m_pAgentThread;
};

#endif // THREADMANAGER_H
