#include "HttpClient.h"
#include "HttpGetAccessToken.h"
#include "HttpCreateMenu.h"


HttpClient* HttpClient::m_pInstance = NULL;

HttpClient *HttpClient::instance()
{
    if(m_pInstance == NULL) {
        m_pInstance = new HttpClient;
    }
    return m_pInstance;
}

void HttpClient::setToken(QString sToken)
{
    m_sToken = sToken;
}

void HttpClient::RequestOnGetAccessToken(QString sAppId, QString sAppSecret)
{
    HttpGetAccessToken *p = new HttpGetAccessToken(sAppId, sAppSecret);
    p->setNetManager(m_pManager);
    p->getRequest();
}

void HttpClient::RequestOnCreateMenu(QList<ButtonMenu_t> buttonList, QString accessToken)
{
    HttpCreateMenu *p = new HttpCreateMenu(buttonList, accessToken);
    p->setNetManager(m_pManager);
    p->postJsonRequest();
}

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
    m_pManager = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{

}
