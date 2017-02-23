/*
 * Licensed Materials - Property of Dasudian
 * Copyright Dasudian Technology Co., Ltd. 2017
 */
//#include "includes.h"
#include "DataHubClient.h"
#include "datahub_demo.h"
#include "DatahubNetwork.h"

#if defined(DATAHUB_DEMO)
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#endif

//#define DATAHUB_CLIENT_PRIO        6
//#define DATAHUB_CLIENT_STK_SIZE    300
//OS_STK DATAHUB_CLIENT_TASK_STK[DATAHUB_CLIENT_STK_SIZE];
//
// datahub sdk demo
static unsigned char buf[100];
static unsigned char readbuf[100];
//#define TEST_DATAHUB_CLIENT_PRIO 12
//#define TEST_DATAHUB_TASK_STK_SIZE 500
//OS_STK TEST_DATAHUB_CLIENT_TASK_STK[TEST_DATAHUB_TASK_STK_SIZE];
//
void self_init(struct self_s *self)
{
    self->socket = -1;
}

static void onMessageReceived(MessageData* md)
{
    printf("onMessageReceived:%.*s\r\n", md->message->payloadlen, (char*)md->message->payload);
}


void data_thread(void *arg)
{
    datahub_client client;
    datahub_message message = {
        "hello world",
        sizeof("hello world"),
    };
    int ret = 0;
    char *topic = "embedded_topic";
    char *instance_id = "dsd_9FmYSNiqpFmi69Bui0_A";// instance_id get from Dasudian customer service
    char *instance_key = "238f173d6cc0608a";// instance_key get from Dasudian customer service
    char *user_name = "embedded_user_name";// a name for the client
    char *client_id = "embedded_client_id";// A unique identifier for a client
    datahub_options options = {
        "try.iotdatahub.net",// server address
        1883,// server port
        1,// clean session
        5000,// connect timeout time
        5000,// command timeout time
        onMessageReceived,// message arrived call back function
        buf,// send message buf
        100,// send message buf size
        readbuf,// read message buf
        100,// read message buf size
    };
    Network n;
    struct self_s self;

    self_init(&self);
    NewNetwork(&n, &self);

    // create a instance
    ret = datahub_create(&client, instance_id, instance_key, user_name, client_id, &options);
    while (1) {
        ret = datahub_connect(&client, &n);
        if (ret) {
            printf("datahub_connect ret:%d\r\n", ret);
//            OSTimeDlyHMSM(0, 0, 2, 0);
        } else {
            printf("datahub_connect success\r\n");
            break;
        }
    }

    // subscribe a topic
    ret = datahub_subscribe(&client, topic);
    if (ret) {
        printf("datahub_subscribe failed:%d\r\n", ret);
    } else {
        printf("datahub_subscribe success\r\n");
    }

    // publish a message to a topic
    ret = datahub_publish(&client, topic, &message);
    if (ret) {
        printf("datahub_publish failed:%d\r\n", ret);
    } else {
        printf("datahub_publish success\r\n");
    }

    // unsubscribe a topic
    ret = datahub_unsubscribe(&client, topic);
    if (ret) {
        printf("datahub_unsubscribe failed:%d\r\n", ret);
    } else {
        printf("datahub_unsubscribe success\r\n");
    }

    // continue publish
    while (1) {
        ret = datahub_publish(&client, topic, &message);
        if (ret) {
            printf("datahub_publish failed:%d\r\n", ret);
        } else {
            printf("datahub_publish success\r\n");
        }
//        OSTimeDlyHMSM(0, 0, 2, 0);
    }

    // disconnect with the server
//    datahub_disconnect(&client);
//    OSTaskDel(0);
}
