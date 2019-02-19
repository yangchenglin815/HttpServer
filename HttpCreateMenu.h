/*
 * 说明：Http请求子类（创建菜单栏）
 */
#ifndef HTTPCREATEMENU_H
#define HTTPCREATEMENU_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpstructdata.h"

class HttpCreateMenu : public IHttpReqRes
{
    Q_OBJECT
public:
    explicit HttpCreateMenu(QList<ButtonMenu_t> buttonList, QString accessToken);
protected:
    void processResponse(QString strResult);
    void processError();
private:
    QString createButtonJson(QList<ButtonMenu_t> buttonList);
};

#endif // HTTPCREATEMENU_H
