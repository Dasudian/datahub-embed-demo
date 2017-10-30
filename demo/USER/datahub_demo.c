/*
 * Licensed Materials - Property of Dasudian
 * Copyright Dasudian Technology Co., Ltd. 2017
 */
#include "DataHubClient.h"
#include "datahub_demo.h"
#include "DatahubNetwork.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "ucos_ii.h"
#include <stdarg.h>

/* 发送缓冲区 */
static unsigned char sendbuf[256];
/* 读缓冲区 */
static unsigned char readbuf[256];

/* 如何处理输出 */
void datahub_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* 如果重定向了输出,可以使能该函数 */
//    vprintf(format, ap);
    va_end(ap);
}

/* 初始化socket */
void self_init(struct self_s *self)
{
    self->socket = -1;
}

/* 接收到消息后的回调函数 */
void onMessageReceived(void *context, lenstring *topic, datahub_message *msg)
{
    datahub_client *client = (datahub_client *)context;
    int i;

    printf("receive a message: client [ %p ] topic [ ", client);
    for (i = 0; i< topic->len; i++) {
        printf("%c", topic->data[i]);
    }
    printf(" ] payload [ %.*s ]\r\n", msg->payload_len, (char*)msg->payload);
}

/* 客户端的网络发生变化后,通知用户的回调函数 */
void connection_status_changed(void *context, int isconnected)
{
    datahub_client *client = (datahub_client *)context;
    
    if (isconnected == DATAHUB_FALSE) {
         printf("client %p connection status: disconnected\r\n", client);
    } else {
         printf("client %p connection status: connected\r\n", client);
    }   
}

datahub_client client;

void data_thread(void *arg)
{
    datahub_message message = DATAHUB_MESSAGE_INITIALIZER;
    datahub_options options = DATAHUB_OPTIONS_INITIALIZER;
    int ret, i;
    char *topic = "embedded_topic";/* 订阅的topic */

    /* instance id, 标识客户的唯一ID，请联系大数点商务
     * support@dasudian.com获取
     */
    char *instance_id = "your_instance_id";

    /* instance key, 与客户标识相对应的安全密钥，请联系大数点商务
     * support@dasudian.com获取
     */
    char *instance_key = "your_instance_key";

    /* 大数点IoT DataHub云端地址，请联系大数点商务support@dasudian.com获取 */
    char *server_url = "www.example.com";

    char *client_type = "sensor";/* 客户端的类型 */
    char *client_id = "embedded_client_id";/* 客户端的唯一标识符 */
    Network network;
    struct self_s self;

    /* 初始化消息 */
    message.payload = "{\"context\":\"hello world\"}";
    message.payload_len = strlen("{\"context\":\"hello world\"}");

    /* 设置各个选项 */
    options.host = server_url;
    options.context = &client;
    options.message_received = onMessageReceived;
    options.connection_status_changed = connection_status_changed;
    options.sendbuf = sendbuf;
    options.sendbuf_size = 256;
    options.readbuf = readbuf;
    options.readbuf_size = 256;

    /* 初始化网络 */
    self_init(&self);
    /* 必须调用NewNetwork初始化 */
    NewNetwork(&network, &self);

    /* 创建客户端实例 */
    ret = datahub_create(&client, instance_id, instance_key, client_type, client_id, &network, &options);
    if (ret != ERROR_NONE) {
        printf("create client failed, ret [ %d ]\r\n", ret);
        return;
    }

    /* 订阅topic, 最大以qos1的服务质量接收消息 */
    while (1) {
        ret = datahub_subscribe(&client, topic, 1, 10);
        if (ret) {
            printf("datahub_subscribe failed:%d\r\n", ret);
            OSTimeDlyHMSM(0, 0, 2, 0);
        } else {
            printf("datahub_subscribe success\r\n");
            break;
        }
    }

    /* 发送10个QoS为0的消息(可能到达,也可能不到达) */
    for (i = 0;i < 10; i++) {
        ret = datahub_sendrequest(&client, topic, &message, JSON, 0, 10);
        if (ret) {
            printf("datahub_publish failed:%d\r\n", ret);
        } else {
            printf("datahub_publish success\r\n");
        }
        OSTimeDlyHMSM(0, 0, 2, 0);
    }

    /* 断开连接并销毁客户端 */
    datahub_destroy(&client);
}
