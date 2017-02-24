# 通信接口

```
struct Network
{
   /*
    * 指向用户自定义的用于实现通信接口的结构体，可以通过NewNetwork()函数赋值。可被接口
    * ConnectNetwork()， ucos_read()，ucos_write()，和ucos_disconnect()使用
    */
    void *data;

   /*
    * 一个用于从网络中读取数据的回调函数。指向ucos_read()函数
    */
    int (*read) (Network* Net, unsigned char* buffer, int len, int timeout_ms);

   /*
    * 一个用于向网络发送数据的回调函数。指向ucos_write()函数
    */
    int (*write) (Network* Net, unsigned char* buffer, int len, int timeout_ms);

   /*
    * 一个用于断开网络的回调函数。指向ucos_disconnect()函数
    */
    void (*disconnect) (Network* Net);

#define mqttread read
#define mqttwrite write
};

/*
 * 描述: 连接服务器接口，由datahub_connect()调用
 * 参数:
 *     Net: 指向Network结构体， 使用前需要调用NetworkInit()初始化
 *     addr: 服务器的ip地址或者域名
 *     port: 服务器的服务端口
 * 返回值:
 *    返回0表示成功,否则表示失败
 *
 */
int ConnectNetwork(Network* Net, char* addr, int port);

/*
 * 描述: 从网络中接收数据
 * 参数:
 *     Net: 指向Network结构体， 使用前需要调用NetworkInit()初始化
 *     buffer: 接收缓冲区的首地址
 *     len: 缓冲区的长度
 *     timeout_ms: 读操作的超时时间。如果经过了timeout_ms毫秒还没有数据，则
        函数返回
 * 返回值:
 *     读取成功的数据长度, 值应大于等于0
 *
 */
int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/*
 * 描述: 向网络发送数据, 由datahub_publish(), datahub_subscribe()调用
 * 参数:
 *     Net: 指向Network结构体， 使用前需要调用NetworkInit()初始化
 *     buffer: 发送缓冲区的首地址
 *     len: 缓冲区的长度
 *     timeout_ms: 写操作的超时时间。如果经过了timeout_ms毫秒还没有数据，则
        函数返回
 * 返回值:
 *     发送成功的数据长度, 值应大于等于0
 *
 */
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/*
 * 描述: 断开网络连接,由datahub_disconnect()调用
 * 参数:
 *     Net: 指向Network结构体， 使用前需要调用NetworkInit()初始化
 * 返回值:
 *    无返回值
 *
 */
void ucos_disconnect(Network* Net);
```

# 定时器接口

```
typedef struct Timer {
    /* 用于存储定时器的超时时刻 */
    unsigned long end;
}Timer;

/*
 * 描述: 初始化数据结构Timer
 * 参数:
 *     timer: 指向待初始化的Timer结构体
 * 返回值:
 *    无返回值
 *
 */
void InitTimer(Timer* timer);

/*
 * 描述: 判断定时器是否超时
 * 参数:
 *     timer: 定时器指针
 * 返回值:
 *     1 表示超时，0表示未超时
 *
 */
char expired(Timer* timer);

/*
 * 描述: 增加定时器的时间
 * 参数:
 *     timer: 指向Timer结构体
 *     timeout: 增加的时间，单位为毫秒
 * 返回值:
 *    无返回值
 *
 */
void countdown_ms(Timer* timer, unsigned int timeout);

/*
 * 与countdown_ms()功能相同，只不过countdown()的参数timeout的单位为秒
 *
 */
void countdown(Timer* timer, unsigned int timeout);

/*
 * 描述: 定时器从当前时间开始，剩余多少时间超时
 * 参数:
 *     timer: 指向Timer结构体
 * 返回值:
 *    剩余的时间，单位为毫秒
 *
 */
int left_ms(Timer* timer);

```

# Datahub接口

```
/*
 * 消息结构体，用于描述一个消息
 */
typedef struct datahub_message {
    /* 指向待发送消息的指针 */
    void *payload;
    /* 消息的长度 */
    unsigned int payload_length;
} datahub_message;

/* 选项结构体，用于设置MQTT协议的相关选项 */
typedef struct datahub_options {
    /* 大数点MQTT服务器域名或地址 */
    char *host;
    /* 大数点MQTT服务器开放端口号 */
    int port;
    /* 是否清除会话，如果不清除会话，则大数点MQTT服务器
     * 记录本次会话(由instance_id 和 client_id 唯一确定)
     * 订阅的topic。当重新连接的时候，不需要重新订阅topic就能
     * 接收到消息
     */
    unsigned char cleansession;
    /* 连接到MQTT服务器的超时时间 */
    unsigned int connect_timeout;
    /* 连接、发送、订阅、取消订阅函数的超时时间 */
    unsigned int command_timeout;
    /* 收到消息后的回调函数
     * messageHandler 的定义如下：
     * typedef void (*messageHandler)(MessageData* md);
     * 其中md->topicName->lenstring.data 是topic的名字，类型为char *
     * md->message->payload为指向消息的指针，类型为void *
     * md->message->payloadlen为消息的大小，类型为size_t
     */
    messageHandler message_handler;

    /* 发送缓冲区的首地址 */
    unsigned char *buf;
    /* 发送缓冲区的大小 */
    int buf_size;
    /* 接收缓冲区的首地址 */
    unsigned char *readbuf;
    /* 接收缓冲区的大小 */
    int readbuf_size;
} datahub_options;

/*
 * 描述: 该函数创建一个客户端实例，该实例可用于连接大数点MQTT服务器
 * 参数:
 *    client: 如果函数成功调用，则会返回一个客户端实例
 *          注意: 不能为空
 *    instance_id: 用于连接大数点服务器的唯一标识，由大数点提供
 *          注意: 不能为空
 *    instance_key: 用于连接大数点服务器的密码，由大数点提供
 *          注意: 不能为空
 *    user_name: 设备的名字
 *          注意: 不能为空
 *    client_id: 设备的id
 *          注意: 不能为空；一个客户可以与服务器建立多条连接，每条连接由instance_id和
 *          client_id唯一确定
 *    options: MQTT的选项。具体包含的选项可以查看datahub_options结构体
 *          注意:不能为空
 * 返回值:
 *      DSD_MQ_SUCCESS 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_create(datahub_client *client, char *instance_id, char *instance_key, 
                    char *user_name, char *client_id, datahub_options *options);

/*
 * 描述: 客户端连接大数点MQTT服务器
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    n: 用于连接网络并发送和接收数据，使用前需要调用InitNetwork()初始化
 *          注意: 不能为空
 * 返回值:
 *      DSD_MQ_SUCCESS 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_connect(datahub_client *client, Network *n);

/*
 * 描述: 查看客户端的连接状态
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 * 返回值:
 *      1 表示连接到服务器，0表示未连接到服务器
 */
int datahub_isconnected(datahub_client *client);

/*
 * 描述: 发送消息(同步)
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 消息对应的topic。如果消息发送前有另一个客户端已经订阅该topic，则
 *          另一个客户端就会收到消息。
 *          注意: 不能为空
 *    msg: 发送的消息
 *          注意: 不能为空
 * 返回值:
 *      DSD_MQ_SUCCESS 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_publish(datahub_client *client, char *topic, datahub_message *msg);

/*
 * 描述: 订阅某一个topic(同步)
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 订阅的topic
 *          注意: 不能为空
 * 返回值:
 *      DSD_MQ_SUCCESS 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_subscribe(datahub_client *client, char *topic);

/*
 * 描述: 取消订阅某一个topic(同步)
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 取消订阅的topic
 *          注意: 不能为空
 * 返回值:
 *      DSD_MQ_SUCCESS 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_unsubscribe(datahub_client *client, char *topic);

/*
 * 描述: 断开连接
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 * 返回值:
 *      DSD_MQ_SUCCESS 表示成功，其他表示错误。错误码请查看开发文档API.md
 */
void datahub_disconnect(datahub_client *client);
```

# 错误代码

```
enum datahub_return_code{ 
    DSD_MQ_ILLEGAL_PARAMETER = -6,// 非法参数
    DSD_NETWORK_UNREACHABLE = -5,// 网络不可用
    DSD_MQ_SERVER_NOTCONNECTED = -4,// 服务器未连接
    DSD_MQ_INVALID_APPID_OR_APPSEC = -3,// instance_id 或 instance_key 不正确
    DSD_MQ_TIMEOUT = -2,// 发送超时
    DSD_MQ_FAILURE = -1,// 发送失败
    DSD_MQ_SUCCESS = 0,// 发送成功
};
```

# Q&A

Q: Program stops at 'BKPT 0xAB' when debugging. Besides, when you check the calling stack, you see function _sys_open(), freopen(), _initio() and main().

A: This could happen when using version 1.x.x of SDK, which uses printf() to print out debug information. This is a bug of SDK. There are two way to solve this problem:1) update SDK to version 2.0.0. 2) rewrite function fputc() then click "Use MicroLib"(Options for Target --> Target --> Code Generation --> click "Use MicroLib")

Q:

...

Error: L6367E: datahubclient.o attributes are not compatible with the provided attributes . Tag_CPU_arch = ARM v7E-M (=13)

...

A: This is because the arch of your CPU doesn't match the library. Please find the appropriate library of your CPU in [lib](./lib). If you don't find what you need, please contact us(support@dasudian.com).

# Support

If there are other problems or advice, Please send a email to support@dasudian.com.
