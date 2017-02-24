/*
 * Licensed Materials - Property of Dasudian 
 * Copyright Dasudian Technology Co., Ltd. 2017 
 */
#ifndef _DATAHUB_CLIENT_H_
#define _DATAHUB_CLIENT_H_

#include "MQTTClient.h"

#define DATAHUB_DEBUG

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

/* 由SDK维护，用户无需了解 */
typedef struct datahub_client {
	char *instance_id;
	char *instance_key;
	char *user_name;
	char *client_id;
	datahub_options *options;
	Network *n;
	Client c;
} datahub_client;

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

#endif
