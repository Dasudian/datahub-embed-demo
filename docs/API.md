# Dasudian IoT DataHub Embedded SDK

- [版本信息](#version)
- [介绍](#introduce)
- [网络接口定义](#network_define)
- [网络接口之连接](#network_connect)
- [网络接口之接收](#network_receive)
- [网络接口之发送](#network_send)
- [网络接口之断开](#network_disconnect)
- [定时器定义](#timer_define)
- [初始化定时器](#timer_init)
- [判断定时器是否超时](#timer_timeout)
- [增加定时器的时间(毫秒)](#timer_add_ms)
- [增加定时器的时间(秒)](#timer_add_s)
- [剩余时间](#timer_left)
- [创建客户端实例](#create)
- [同步发送消息](#send_request)
- [同步订阅主题](#subscribe)
- [同步取消订阅主题](#unsubscribe)
- [销毁客户端](#destroy)
- [检查订阅的数据是否到达,是否应该发送ping包](#yield)
- [选项](#option)
- [错误代码](#error_codes)
- [调试](#debug)

## <a name="version">版本信息</a>

| Date | Version | Note |
|---|---|---|
| 3/31/2017 | 3.0.0 | 根据SDK的标准修改API |
| 不详 | 2.0.1 | 暴露datahub_yield()接口修复不发送数据也无法接收数据的bug |
| 不详 | 2.0.0 | 删除与操作系统有关的源码，适配不同的操作系统;新增定时器接口;不再支持自动重连 |
| 不详 | 1.1.1 | 修复无法重连的bug |
| 不详 | 1.1.0 | 新增网络驱动接口 |
| 不详 | 1.0.0 | 第一版本 |

## <a name="introduce">介绍</a>

SDK基于MQTT协议，传输实时的消息到大数点IoT云服务器，所有的API都是同步的。

你可以收集设备上的数据发送到云上。也可以订阅某个topic，来接收云服务器推送的消息。

如何使用SDK:

- 创建一个客户端实例

- 如果想接收消息，那么就订阅某个topic

- 或者发送消息到服务器

- 退出时，销毁该客户端

注意: 要循环调用datahub_yield(),用于收数据和发送ping包,否则可能表现为订阅了某个topic
仍旧没有数据达到或者网络断开后客户端仍然显示已连接状态

# 通信接口


## <a name="network_define">网络接口定义</a>

```
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
```

## <a name="network_connect">网络接口之连接</a>

```
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
```


## <a name="network_receive">网络接口之接收</a>

```
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
```


## <a name="network_send">网络接口之发送</a>

```
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
```


## <a name="network_disconnect">网络接口之断开</a>

```
/*
 * 描述: 断开与服务器的连接
 * 参数:
 *  Net: 一个指向Network的结构体指针
 * 返回值:
 *  无
 */
void ucos_disconnect(Network* Net);
```

# 定时器接口

## <a name="timer_define">定时器定义</a>


```
typedef struct Timer {
    /* 用于存储定时器的超时时刻 */
    unsigned long end;
}Timer;
```

## <a name="timer_init">初始化定时器</a>

```
/*
 * 描述: 初始化数据结构Timer
 * 参数:
 *     timer: 指向待初始化的Timer结构体
 * 返回值:
 *    无返回值
 *
 */
void InitTimer(Timer* timer);
```

## <a name="timer_timeout">判断定时器是否超时</a>


```
/*
 * 描述: 判断定时器是否超时
 * 参数:
 *     timer: 定时器指针
 * 返回值:
 *     1 表示超时，0表示未超时
 *
 */
char expired(Timer* timer);
```

## <a name="timer_add_ms">增加定时器的时间(毫秒)</a>


```
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
```

## <a name="timer_add_s">增加定时器的时间(秒)</a>


```
/*
 * 描述: 增加定时器的时间
 * 参数:
 *     timer: 指向Timer结构体
 *     timeout: 增加的时间，单位为秒
 * 返回值:
 *    无返回值
 *
 */
void countdown(Timer* timer, unsigned int timeout);
```

## <a name="timer_left">剩余时间</a>


```
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

## <a name="create">创建客户端实例</a>


```
/*
 * 描述: 该函数创建一个客户端实例，该实例可用于连接大数点MQTT服务器
 * 参数:
 *    client: 如果函数成功调用，则客户端实例可用于发送数据和订阅主题等
 *          注意: 不能为空
 *    instance_id: 用于连接大数点服务器的唯一标识，由大数点提供
 *          注意: 不能为空
 *    instance_key: 用于连接大数点服务器的密码，由大数点提供
 *          注意: 不能为空
 *    client_name: 设备的名字
 *          注意: 不能为空
 *    client_id: 设备的id
 *          注意: 不能为空；一个客户可以与服务器建立多条连接，每条连接由instance_id和
 *          client_id唯一确定
 *    network: 网络接口，由用户实现网络数据的收发，SDK会调用这些函数
 *          注意：不能为空且传入前需要调用NewNetwork()初始化,否则会出现未定义的错误
 *    options: 可选的选项。具体包含的选项可以查看datahub_options结构体
 *          注意:不能为空
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_create(datahub_client *client, char *instance_id, char *instance_key, 
					char *client_name, char *client_id, Network *network, datahub_options *options);
```

## <a name="send_request">同步发送消息</a>

```
/*
 * 描述: 同步发送消息
 * 参数:
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    topic: 消息对应的topic。如果消息发送前另一个客户端订阅该topic，则
 *          另一个客户端就会收到消息。
 *          注意: 不能为空
 *    message: 发送的消息
 *          注意: 不能为空
 *    QoS: 消息的服务质量
 *          0   消息可能到达，也可能不到达
 *          1   消息一定会到达，但可能会重复，当然，前提是返回ERROR_NONE
 *          2   消息一定会到达，且只到达一次，当然，前提是返回ERROR_NONE
 *    timeout: 函数阻塞的最大时间,单位为秒
 *          注意：必须 > 0
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_sendrequest(datahub_client *client, char *topic, datahub_message *message, int QoS, int timeout);
```

## <a name="subscribe">同步订阅主题</a>


```
/*
 * 描述: 同步订阅主题
 * 参数:
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    topic: 订阅的topic
 *          注意: 不能为空
 *    timeout: 函数阻塞的最大时间,单位为秒
 *          注意：必须 > 0
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_subscribe(datahub_client *client, char *topic, int timeout);
```

## <a name="unsubscribe">同步取消订阅主题</a>


```
/*
 * 描述: 同步取消订阅topic
 * 参数:
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    topic: 取消的topic
 *          注意: 不能为空
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_unsubscribe(datahub_client *client, char *topic, int timeout);
```

## <a name="destroy">销毁客户端</a>


```
/*
 * 描述: 销毁客户端,销毁后连接不存在,不能收发数据
 * 参数:
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    timeout: 函数阻塞的最大时间,单位为秒
 *          注意：必须 > 0,否则,采用5秒(主要用于断开连接)
 * 返回值:
 *    无
 */
void datahub_destroy(datahub_client *client);
```

## <a name="yield">检查订阅的数据是否到达,是否应该发送ping包</a>


```
/*
 * 描述：花费一定时间检测订阅的数据是否到达以及判断是否应该发送ping包
 * 参数：
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    timeout_ms: 函数阻塞的最大时间
 *          注意：单位为毫秒
 * 返回值：
 *    ERROR_NONE 表示操作成功，其他表示错误。错误码请查看开发文档API.md
 */
int datahub_yield(datahub_client *client, int timeout_ms);
```

## <a name="option">选项</a>


```
/* 选项结构体，用于设置MQTT协议的相关选项 */
typedef struct datahub_options {
    /* 大数点MQTT服务器域名或地址, 默认为 try.iotdatahub.net */
	char *host;
    /* 大数点MQTT服务器开放端口号, 默认为1883 */
	int port;

    /*
     * 传递给回调函数message_received()和connection_status_changed(), 即回调函数
     * 的第一个参数context, 默认为NULL
     */
    void *context;
    /* 收到消息后的回调函数, 默认为NULL*/
    MESSAGE_RECEIVED *message_received;
    /* 当网络状态发生变化时, 通知用户的回调函数, 默认为NULL */
    CONNECTION_STATUS_CHANGED *connection_status_changed;
	
    /* 发送缓冲区的首地址, 必须由用户手动设置, 默认为NULL*/
	unsigned char *sendbuf;
    /* 发送缓冲区的大小, 必须由用户手动设置, 默认为0 */
	int sendbuf_size;
    /* 接收缓冲区的首地址, 必须由用户手动设置, 默认为NULL*/
	unsigned char *readbuf;
    /* 接收缓冲区的大小, 必须由用户手动设置, 默认为0 */
	int readbuf_size;
} datahub_options;

/*
 * 消息结构体，用于描述一个消息
 */
typedef struct datahub_message {
    /* 指向待发送的消息 */
	void *payload;
    /* 消息的长度 */
	unsigned int payload_len;
} datahub_message;

/* 字符串,不以'\0'结尾 */
typedef struct lenstring_s {
    /* 字符串的起始位置 */
   char *data;
   /* 字符串的长度 */
   int len;
}lenstring;

/* 描述:收到消息后的回调函数
 * 参数:
 *  context: 在选项中设置的context
 *  topic: 消息对应的主题
 *  msg: 描述消息的结构体
 */
typedef void MESSAGE_RECEIVED(void *context, lenstring *topic, datahub_message *msg);

/* 描述: 当网络状态发生变化时,通知用户的回调函数
 *   注意:该函数只用于通知用户网络状态,请不要在函数中进行耗时操作或者阻塞
 * 参数:
 *  context: 在选项中设置的context
 *  isconnected: DATAHUB_TRUE表示已连接, DATAHUB_FALSE表示已断开
 */
typedef void CONNECTION_STATUS_CHANGED(void *context, int isconnected);


```

## <a name="error_codes">错误代码</a>


```
enum datahub_error_code_s {
    /*
     * 返回码: 成功
     */
    ERROR_NONE = 0,
    /*
     * 返回码: 某些参数不合法
     */
    ERROR_ILLEGAL_PARAMETERS = -1,
    /*
     * 返回码: 客户端已断开
     */
    ERROR_DISCONNECTED = -2,
    /*
     * 返回码: MQTT服务器不支持当前使用的协议版本号,请联系开发人员
     */
    ERROR_UNACCEPT_PROTOCOL_VERSION = -3,
    /*
     * 返回码: client_id不可用,可能使用了不支持的字符
     */
    ERROR_IDENTIFIER_REJECTED = -4,
    /*
     * 返回码: 服务器不可用
     */
    ERROR_SERVER_UNAVAILABLE = -5,
    /*
     * 返回码: instance_id 或者instance_key不正确,请检查
     */
    ERROR_BAD_USERNAME_OR_PASSWD = -6,
    /*
     * 返回码: 未被授权
     */
    ERROR_UNAUTHORIZED = -7,
    /*
     * 返回码: 验证服务器不可用
     */
    ERROR_AUTHORIZED_SERVER_UNAVAILABLE = -8,
    /*
     * 返回码: 操作失败
     */
    ERROR_OPERATION_FAILURE = -9,
    /*
     * 返回码: 消息过长
     */
    ERROR_MESSAGE_TOO_BIG = -10,
    /*
     * 返回码: 网络不可用
     */
    ERROR_NETWORK_UNREACHABLE = -11,
    /*
     * 返回码: 超时
     */
    ERROR_TIMEOUT = -12,
    /*
     * 返回码: 非法的选项
     */
    ERROR_ILLEGAL_OPTION = -200

};
```

## <a name="debug">调试</a>

```

为了方便调试, 我们输出了重要的调试信息, 用户可以重定向这些输出.
正式上线的环境下,可以将datahub_printf()置空,忽略这些输出
#include <stdarg.h>
void datahub_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* 如果重定向了输出,可以取消注释*/
//  vprintf(format, ap);
    va_end(ap);
}
```
