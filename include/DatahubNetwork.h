#ifndef DATAHUB_NETWORK_H
#define DATAHUB_NETWORK_H

typedef struct Network Network;

/**
 * @brief 描述网络的结构体
 *
 * @note 如果网络接口和client实例在不同的Task中运行, 则网络接口任务优先级要高
 * 于调用client实例的Task; 否则, 调用所有接口都会超时, 数据却可能发送成功
 */
struct Network
{
    /**
     * @brief 一个指向用户自定义的结构体指针.
     *
     * 由函数NewNetwork初始化. 函数ConnectNetwork, read, write, 和disconnect可使
     * 用<br>
     */
    void *data;

    /**
     * @brief 读取网络数据的函数指针.
     *
     * SDK将调用该接口从网络收数据. 需要用户实现并赋值<br>
     */
    int (*read) (Network* Net, unsigned char* buffer, int len,
            int timeout_ms);

    /**
     * @brief 发送网络数据的函数指针.
     *
     * SDK将调用该接口向网络发送数据. 需要用户实现并赋值<br>
     */
    int (*write) (Network* Net, unsigned char* buffer, int len,
            int timeout_ms);

    /**
     * @brief 断开网络的函数指针.
     *
     * SDK将调用该接口断开与服务器的连接. 需要用户实现并赋值<br>
     */
    void (*disconnect) (Network* Net);

#define mqttread read
#define mqttwrite write
};

/**
 * @brief 从网络中读取数据
 *
 * @param Net 一个指向Network的结构体指针
 * @param buffer 读缓冲区的首地址
 * @param len SDK需要读取的数据长度
 * @param timeout_ms 读操作的最大阻塞时间, 单位为毫秒
 * @return 返回读取的数据长度, 正常情况下为参数len表示的大小,
 * 其他情况表示失败<br>
 * @note 当SDK还未读取数据而4G模块已经接收到数据, 用户需要保存数据, 等待
 * SDK读取<br>
 * 此外, SDK读取的数据长度是通过参数len指定的, 而不是返回所有接收到的数据
 */
int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/**
 * @brief 向网络中发送数据
 * @param Net 一个指向Network的结构体指针
 * @param buffer 发送缓冲区的首地址
 * @param len 发送数据的大小
 * @param timeout_ms 最大阻塞时间, 单位为毫秒
 * @return 返回写入网络中的数据长度, 正常情况下为参数len表示的大小, 其他情况表
 * 示失败<br>
 * @note 成功是指用户将数据全部发送到网络上, 而不是仅仅交付给传输层而未发送到
 * 网络上;<br>
 * 比如说通过串口发送给4G模块后, 4G模块只是保留了用户发送的数据且还未完全传输到
 * 网络上, 用户需要等待4G模块发送完成后才能返回<br>
 */
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/**
 * @brief 断开与服务器的连接
 * @param Net 一个指向Network的结构体指针
 * @return 无
 */
void ucos_disconnect(Network* Net);

/**
 * @brief 初始化Network结构体
 * @param Net 一个指向Network的结构体指针
 * @param data 一个指向用户自定义的结构体指针
 * @return 无
 */
void NewNetwork(Network* Net, void *data);

/**
 * @brief 连接服务器
 * @param Net 一个指向Network的结构体指针
 * @param addr 服务器的ip或者域名
 * @param port 服务器的端口号
 * @return 0 表示成功, 其他表示错误
 */
int ConnectNetwork(Network* Net, char* addr, int port);

#endif
