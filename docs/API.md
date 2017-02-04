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
