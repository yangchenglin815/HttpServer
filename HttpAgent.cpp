#include "HttpAgent.h"
#include "HttpClient.h"
#include "ThreadManager.h"

HttpAgent* HttpAgent::m_pInstance = NULL;

HttpAgent *HttpAgent::instance()
{
    if(m_pInstance == NULL) {
        m_pInstance = new HttpAgent;
    }
    return m_pInstance;
}

void HttpAgent::RequestOnGetAccessToken(QString sAppId, QString sAppSecret)
{
    HttpClient::instance()->RequestOnGetAccessToken(sAppId, sAppSecret);
}

void HttpAgent::RequestOnCreateMenu(QList<ButtonMenu_t> buttonList, QString accessToken)
{
    HttpClient::instance()->RequestOnCreateMenu(buttonList, accessToken);
}

HttpAgent::HttpAgent(QObject *parent) : QObject(parent)
{
    initConnect();
    moveToThread(ThreadManager::getAgentThread());
    ThreadManager::getAgentThread()->start();
}

HttpAgent::~HttpAgent()
{

}

void HttpAgent::initConnect()
{
    connect(HttpSignal::instance(), SIGNAL(sigOnGetAccessToken(int, QString, QString)), this, SIGNAL(sigOnGetAccessToken(int, QString, QString)));
    connect(HttpSignal::instance(), SIGNAL(sigOnCreateMenu(int, QString)), this, SIGNAL(sigOnCreateMenu(int, QString)));
}
