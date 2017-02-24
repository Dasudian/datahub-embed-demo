# 通信接口

嵌入式SDK依赖通信接口的实现，所以，用户必须先实现定义在DatahubNetwork.h中的接口，包括ConnectNetwork(),ucos_read(),ucos_write()和ucos_disconnect()。下面详细介绍各个接口的功能和参数

## 通信接口的数据结构

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

```

## 连接服务器

```
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
```

## 接收数据

```
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
```

## 发送数据

```
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
```

## 断开连接

```
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

## 创建datahub client

创建一个client，可以设置各种mqtt的选项。

```
int datahub_create(datahub_client *client, char *instance_id, char *instance_key, 
					char *user_name, char *client_id, datahub_options *options);
```

## 连接服务器

同步方法

```
int datahub_connect(datahub_client *client);
```

## 获取sdk当前连接状态

```
datahub_connect_status datahub_isconnected(datahub_client *client);
```

## 同步发送消息

```
int datahub_publish(datahub_client *client, char *topic, datahub_message *msg);
```

## 订阅

同步方法

```
int datahub_subscribe(datahub_client *client, char *topic);

```

## 取消订阅

同步方法

```
int datahub_unsubscribe(datahub_client *client, char *topic);
```

## 断开连接

同步方法

```
void datahub_disconnect(datahub_client *client);
```

## 错误代码

```
enum datahub_return_code{ 
	DSD_MQ_ILLEGAL_PARAMETER = -6,// 非法参数
	DSD_NETWORK_UNREACHABLE = -5,// 网络不可用
	DSD_MQ_SERVER_NOTCONNECTED = -4,// 服务器未连接
	DSD_MQ_INVALID_APPID_OR_APPSEC = -3,// 
	DSD_MQ_TIMEOUT = -2,// 发送超时
	DSD_MQ_FAILURE = -1,// 发送失败
	DSD_MQ_SUCCESS = 0,// 发送成功
};
```

## 数据结构详细文档

```
// publish message 定义
typedef struct datahub_message {
	// 消息内容
	void *payload;
	// 内容的长度
	unsigned int payload_length;
} datahub_message;

// 连接状态定义
typedef enum datahub_connect_status {
	// 当前和服务器连接正常
	DATAHUB_CONNECTED,
	// SDK正在重连
	DATAHUB_RECONNECTING,
	// 与服务器连接断开了
	DATAHUB_DISCONNECTED
} datahub_connect_status;

// 
typedef struct datahub_options {
	// datahub server host
	char *host;

	// datahub server port
	int port;

	// 是否清除会话，如果设置为1，则在客户端断开后，订阅的topic也会消息。
	// 如果设置为0，则断开连接后，服务器会保存订阅的topic。
	unsigned char cleansession;

	// 连接超时时间，单位毫秒
	unsigned int connect_timeout;

	// 发送消息超时时间，单位毫秒
	unsigned int command_timeout;

	// a callback function to recive message
	messageHandler message_handler;
	
	// write and read buffer
	unsigned char *buf;

	// write buffer size
	int buf_size;

	// read buf address
	unsigned char *readbuf;

	// read buffer size
	int readbuf_size;
	
	// thread options
	OS_STK *task_stack;

	// UCOS task size
	int stack_size;

	// UCOS task priority 
	int prio;
} datahub_options;

typedef struct datahub_client {
	// datahub instance_id, get from Dasudian Customer service
	char *instance_id;
	// datahub instance_key, get from Dasudian Customer service
	char *instance_key;
	// the device name
	char *user_name;
	// the device unique id, different device must use different client_id
	char *client_id;
	// sdk config options
	datahub_options *options;

	// 下面的值都有sdk内部维护，不要在外部修改
	datahub_connect_status status;
	Network n;
	Client c;
	void *message_group[DATAHUB_QUEUE_SIZE];
	OS_EVENT *message_queue;
	unsigned int current_reconnect_rate;
} datahub_client;
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
