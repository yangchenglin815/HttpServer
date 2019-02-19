/*
 * 说明：枚举类
 */
#ifndef ENUMDATA_H
#define ENUMDATA_H

//网络请求码
enum ServerHttpErrorCode {
    e_connectServerError = -1,    //连接服务器失败
    e_success = 0,                //连接服务器成功
    e_paramError = 1,             //请求参数错误
};

#endif // ENUMDATA_H
