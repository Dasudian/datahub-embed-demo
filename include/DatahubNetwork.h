#ifndef DATAHUB_NETWORK_H
#define DATAHUB_NETWORK_H

typedef struct Network Network;

struct Network
{
    /*
     * 一个指向用户自定义的结构体指针. 由NewNetwork()初始化. 函数ConnectNetwork(),
     * read(), write(), 和disconnect()可使用
     */
    void *data;

    /*
     * 读函数指针. SDK将调用该接口从网络收数据. 需要用户实现并赋值
     */
    int (*read) (Network* Net, unsigned char* buffer, int len, int timeout_ms);


    /*
     * 写函数指针. SDK将调用该接口向网络发送数据. 需要用户实现并赋值
     */
    int (*write) (Network* Net, unsigned char* buffer, int len, int timeout_ms);

    /*
     * 断开网络的函数指针. SDK将调用该接口断开与服务器的连接. 需要用户实现并赋值
     */
    void (*disconnect) (Network* Net);

#define mqttread read
#define mqttwrite write
};

/*
 * 描述: 从网络中读取数据
 * 参数:
 *  Net: 一个指向Network的结构体指针
 *  buffer: 读缓冲区的首地址
 *  len: 读缓冲区的大小
 *  timeout_ms: 读操作的最大阻塞时间, 单位为毫秒
 * 返回值:
 *  成功读取数据的大小, -1 表示失败
 */
int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/*
 * 描述: 向网络中发送数据
 * 参数:
 *  Net: 一个指向Network的结构体指针
 *  buffer: 发送缓冲区的首地址
 *  len: 发送缓冲区的大小
 *  timeout_ms: 发送操作的最大阻塞时间, 单位为毫秒
 * 返回值:
 *  成功发送数据的大小, -1 表示失败
 */
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/*
 * 描述: 断开与服务器的连接
 * 参数:
 *  Net: 一个指向Network的结构体指针
 * 返回值:
 *  无
 */
void ucos_disconnect(Network* Net);

/*
 * 描述: 初始化Network结构体
 * 参数:
 *  Net: 一个指向Network的结构体指针
 *  data: 一个指向用户自定义的结构体指针
 * 返回值:
 *  无
 */
void NewNetwork(Network* Net, void *data);

/*
 * 描述: 连接服务器
 * 参数:
 *  Net: 一个指向Network的结构体指针
 *  addr: 服务器的ip或者域名
 *  port: 服务器的端口号
 * 返回值:
 *  0 表示成功, 其他表示错误
 */
int ConnectNetwork(Network* Net, char* addr, int port);

#endif

