/*
 * Licensed Materials - Property of Dasudian
 * Copyright Dasudian Technology Co., Ltd. 2017
 */
#include "includes.h"
#include "DataHubClient.h"
#include "datahub_demo.h"
#include "DatahubNetwork.h"

#if defined(DATAHUB_DEMO)
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#endif

#if defined(DATAHUB_DEMO)
int errno;
#endif

void NewNetwork(Network* n, void *data)
{
    n->data = data;
    n->mqttread = ucos_read;
    n->mqttwrite = ucos_write;
    n->disconnect = ucos_disconnect;
}

int ConnectNetwork(Network* n, char* addr, int port)
{
#if defined(DATAHUB_DEMO)
    int type = SOCK_STREAM;
    struct sockaddr_in address;
    int rc = -1;
    int family = AF_INET;
    struct addrinfo *result = NULL;
    struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};
    struct self_s *self = (struct self_s *)n->data;

    if ((rc = getaddrinfo(addr, NULL, &hints, &result)) == 0)
    {
        struct addrinfo* res = result;

        /* prefer ip4 addresses */
        while (res)
        {
            if (res->ai_family == AF_INET)
            {
                result = res;
                break;
            }
            res = res->ai_next;
        }

        if (result->ai_family == AF_INET)
        {
            address.sin_port = htons(port);
            address.sin_family = family = AF_INET;
            address.sin_addr = ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
        }
        else
            rc = -1;

        freeaddrinfo(result);
    }

    if (rc == 0)
    {
        self->socket = socket(family, type, 0);
        if (self->socket != -1)
        {
            rc = connect(self->socket, (struct sockaddr*)&address, sizeof(address));
        }
    }

    return rc;
#endif
		return 0;
}
/**
读取多少个字节的数据
buffer：用来接收读取到的数据
len：想要读取多少数据
timeout_ms：读取数据在多少毫秒后超时
*/
int ucos_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
#if defined(DATAHUB_DEMO)
    int bytes = 0;
    struct self_s *self = (struct self_s *)n->data;

    setsockopt(self->socket, SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout_ms, sizeof(int));

    while (bytes < len)
    {
        int rc = recv(self->socket, &buffer[bytes], (size_t)(len - bytes), 0);
        if (rc == 0) {
            //printf("ucos_read: rc == 0\r\n");
        }
        if (rc == -1)
        {
            //printf("ucos_read:rc == -1, error = %d\r\n", errno);
            if (errno != ENOTCONN && errno != ECONNRESET)
            {
                bytes = -1;
                break;
            }
        }
        else
            bytes += rc;
    }
    return bytes;
#endif
		return 0;
}

int ucos_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
#if defined(DATAHUB_DEMO)
    int rc;
    struct self_s *self = (struct self_s *)n->data;

    setsockopt(self->socket, SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout_ms, sizeof(int));
    rc = write(self->socket, buffer, len);
    return rc;
#endif
		return 0;
}

void ucos_disconnect(Network* n)
{
#if defined(DATAHUB_DEMO)
    struct self_s *self = (struct self_s *)n->data;

    close(self->socket);
#endif
}


#define DATAHUB_CLIENT_PRIO        6
#define DATAHUB_CLIENT_STK_SIZE    300
OS_STK DATAHUB_CLIENT_TASK_STK[DATAHUB_CLIENT_STK_SIZE];

// datahub sdk demo
static unsigned char buf[100];
static unsigned char readbuf[100];
#define TEST_DATAHUB_CLIENT_PRIO 12
#define TEST_DATAHUB_TASK_STK_SIZE 500
OS_STK TEST_DATAHUB_CLIENT_TASK_STK[TEST_DATAHUB_TASK_STK_SIZE];

void self_init(struct self_s *self)
{
    self->socket = -1;
}

static void onMessageReceived(MessageData* md)
{
    printf("onMessageReceived:%.*s\r\n", md->message->payloadlen, (char*)md->message->payload);
}


static void data_thread(void *arg)
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
        TEST_DATAHUB_CLIENT_TASK_STK,// datahub sdk task
        TEST_DATAHUB_TASK_STK_SIZE,// datahub sdk task size
        TEST_DATAHUB_CLIENT_PRIO// datahub sdk task prio
    };
    Network n;
    struct self_s self;

    self_init(&self);
    NewNetwork(&n, &self);
    while (1) {
        ret = ConnectNetwork(&n, options.host, options.port);
        if (ret) {
            printf("ConnectNetwork ret: %d\r\n", ret);
        } else {
            printf("ConnectNetwork success\r\n");
            break;
        }

    }

    // create a instance
    ret = datahub_create(&client, instance_id, instance_key, user_name, client_id, &options);
    while (1) {
        ret = datahub_connect(&client, &n);
        if (ret) {
            printf("datahub_connect ret:%d\r\n", ret);
            OSTimeDlyHMSM(0, 0, 2, 0);
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
        OSTimeDlyHMSM(0, 0, 2, 0);
    }

    // disconnect with the server
    datahub_disconnect(&client);
    OSTaskDel(0);
}

INT8U create_datahub_task(void)
{
    INT8U res;
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    res = OSTaskCreate(data_thread,(void*)0,(OS_STK*)&DATAHUB_CLIENT_TASK_STK[DATAHUB_CLIENT_STK_SIZE-1],DATAHUB_CLIENT_PRIO);
    OS_EXIT_CRITICAL();

    return res;
}



