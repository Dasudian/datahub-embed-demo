/*
 * Licensed Materials - Property of Dasudian
 * Copyright Dasudian Technology Co., Ltd. 2017
 */
//#include "includes.h"
#include "DataHubClient.h"
#include "datahub_demo.h"
#include "DatahubNetwork.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "ucos_ii.h"
#include <stdarg.h>


//#define DATAHUB_CLIENT_PRIO        6
//#define DATAHUB_CLIENT_STK_SIZE    300
//OS_STK DATAHUB_CLIENT_TASK_STK[DATAHUB_CLIENT_STK_SIZE];
//
// datahub sdk demo
static unsigned char sendbuf[100];
static unsigned char readbuf[100];
//#define TEST_DATAHUB_CLIENT_PRIO 12
//#define TEST_DATAHUB_TASK_STK_SIZE 500
//OS_STK TEST_DATAHUB_CLIENT_TASK_STK[TEST_DATAHUB_TASK_STK_SIZE];

/* 如何处理输出 */
void datahub_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* 如果重定向了输出,可以使能该函数 */
    vprintf(format, ap);
    va_end(ap);
}

void self_init(struct self_s *self)
{
    self->socket = -1;
}

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
    int ret;
    char *topic = "embedded_topic1";
    char *instance_id = "dsd_9FmYSNiqpFmi69Bui0_A";// instance_id get from Dasudian customer service
    char *instance_key = "238f173d6cc0608a";// instance_key get from Dasudian customer service
    char *user_name = "embedded_user_name";// a name for the client
    char *client_id = "embedded_client_id1";// A unique identifier for a client
    Network network;
    struct self_s self;


    /* 初始化消息 */
    message.payload = "hello world";
    message.payload_len = sizeof("hello world");

    /* 设置各个选项 */
    options.context = &client;
    options.message_received = onMessageReceived;
    options.connection_status_changed = connection_status_changed;
    options.sendbuf = sendbuf;
    options.sendbuf_size = 100;
    options.readbuf = readbuf;
    options.readbuf_size = 100;

    /* 初始化网络 */
    self_init(&self);
    NewNetwork(&network, &self);

    /* 等待网络初始化完成 */
//    OSTimeDlyHMSM(0, 0, 5, 0);
    // create a instance
    ret = datahub_create(&client, instance_id, instance_key, user_name, client_id, &network, &options);
    if (ret != ERROR_NONE) {
        printf("create client failed, ret [ %d ]\r\n", ret);
        return;
    }

    // subscribe a topic
    while (1) {
        ret = datahub_subscribe(&client, topic, 10);
        if (ret) {
            printf("datahub_subscribe failed:%d\r\n", ret);
            OSTimeDlyHMSM(0, 0, 2, 0);
        } else {
            printf("datahub_subscribe success\r\n");
            break;
        }
    }

//    // unsubscribe a topic
//    ret = datahub_unsubscribe(&client, "unused_topic", 10);
//    if (ret) {
//        printf("datahub_unsubscribe failed:%d\r\n", ret);
//    } else {
//        printf("datahub_unsubscribe success\r\n");
//    }

    // continue publish
//    for (i = 0;i < 10; i++) {
      while (1) {
        ret = datahub_sendrequest(&client, topic, &message, 2, 10);
        if (ret) {
            printf("datahub_publish failed:%d\r\n", ret);
        } else {
            printf("datahub_publish success\r\n");
        }
        datahub_yield(&client, 2000);
    }

    datahub_destroy(&client);
    // disconnect with the server
//    datahub_disconnect(&client);
//    OSTaskDel(0);
}
