/*
 * Licensed Materials - Property of Dasudian 
 * Copyright Dasudian Technology Co., Ltd. 2017 
 */
#ifndef _DATAHUB_CLIENT_H_
#define _DATAHUB_CLIENT_H_

#include "MQTTClient.h"

#define DATAHUB_FALSE 0
#define DATAHUB_TRUE 1

#define DEFAULT_HOST           "try.iotdatahub.net"
#define DEFAULT_PORT            1883
#define DEFAULT_CONTEXT         NULL
#define DEFAULT_CALLBACK        NULL
#define DEFAULT_BUF             NULL
#define DEFAULT_BUF_SIZE        0

/* 选项的初始化宏 */
#define DATAHUB_OPTIONS_INITIALIZER {\
    DEFAULT_HOST,\
    DEFAULT_PORT,\
    DEFAULT_CONTEXT,\
    DEFAULT_CALLBACK,\
    DEFAULT_CALLBACK,\
    DEFAULT_BUF,\
    DEFAULT_BUF_SIZE,\
    DEFAULT_BUF,\
    DEFAULT_BUF_SIZE,\
}

/* 消息的初始化宏,默认为"" */
#define DATAHUB_MESSAGE_INITIALIZER {\
    "",\
    1\
}

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

/* 由SDK维护，用户无需了解 */
typedef struct datahub_client {
	char *instance_id;
	char *instance_key;
	char *client_name;
	char *client_id;
	datahub_options *options;
	Network *network;
	Client c;
} datahub_client;

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

/*
 * 描述: 同步取消订阅topic
 * 参数:
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    topic: 取消的topic
 *          注意: 不能为空
 *    timeout: 函数阻塞的最大时间,单位为秒
 *          注意：必须 > 0
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。错误码请查看开发文档API.md
 *
 */
int datahub_unsubscribe(datahub_client *client, char *topic, int timeout);

/*
 * 描述: 销毁客户端,销毁后连接不存在,不能收发数据
 * 参数:
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 * 返回值:
 *    无
 */
void datahub_destroy(datahub_client *client);

/*
 * 描述：花费一定时间检测订阅的数据是否到达以及判断是否应该发送ping包
 * 参数：
 *    client: 由函数datahub_create()成功创建的客户端实例
 *          注意: 不能为空
 *    timeout_ms: 函数阻塞的最大时间
 *          注意：单位为毫秒, 必须大于0
 * 返回值：
 *    ERROR_NONE 表示操作成功，其他表示错误。错误码请查看开发文档API.md
 */
int datahub_yield(datahub_client *client, int timeout_ms);

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

#endif
