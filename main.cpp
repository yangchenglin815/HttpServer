#include <QCoreApplication>
#include "HttpServer.h"

/*  程序入口，任何c++程序都是从main函数开始的    */
int main(int argc, char *argv[])
{
    //创建一个程序实例
    QCoreApplication a(argc, argv);

    //初始化日志，设置日志的路径
    QString path = a.applicationDirPath() + "/log";
    HttpServer::StartLog(path.toLocal8Bit().data());

    //创建服务对象，以8080作为程序端口启动服务程序
    HttpServer ser;
    ser.run(8080);

    //exec()阻塞程序，等待程序主动退出
    return a.exec();
}
