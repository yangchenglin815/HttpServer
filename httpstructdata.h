/*
 * 说明：Http网络数据交换字典
 */
#ifndef HTTPSTRUCTDATA_H
#define HTTPSTRUCTDATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>

//微信端XML数据类型
typedef struct EventData{
    QString ToUserName;
    QString FromUserName;
    QString MsgType;
    QString Event;
    QString Content;
    QString CreateTime;
}EventData_t;
Q_DECLARE_METATYPE(EventData)

//菜单栏数据
typedef struct ButtonMenu{
    QMap<QString, QString> buttonMap;
    QList<ButtonMenu> subButtonList;
}ButtonMenu_t;
Q_DECLARE_METATYPE(ButtonMenu)

//
typedef struct AdvertisementInfo{
    QString sDetial;
    QString sTitle;
    QString sImage;
    QString sUrl;
}AdvertisementInfo_t;
Q_DECLARE_METATYPE(AdvertisementInfo)


#endif // HTTPSTRUCTDATA_H
