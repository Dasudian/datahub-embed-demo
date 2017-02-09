/*
 * Licensed Materials - Property of Dasudian 
 * Copyright Dasudian Technology Co., Ltd. 2017 
 */
#ifndef _DATAHUB_CLIENT_H_
#define _DATAHUB_CLIENT_H_

#include "MQTTClient.h"
#include "MQTTUcos.h"

#define DATAHUB_DEBUG
#define DATAHUB_QUEUE_SIZE 1

typedef struct datahub_message {
	void *payload;
	unsigned int payload_length;
} datahub_message;

typedef enum datahub_connect_status {
	DATAHUB_CONNECTED,
	DATAHUB_RECONNECTING,
	DATAHUB_DISCONNECTED
} datahub_connect_status;

typedef struct datahub_options {
	// datahub server host
	char *host;
	// datahub server port
	int port;
	unsigned char cleansession;
	unsigned int connect_timeout;
	unsigned int command_timeout;
	// a callback function to recive message
	messageHandler message_handler;
	
	// write and read buf
	unsigned char *buf;
	int buf_size;
	unsigned char *readbuf;
	int readbuf_size;
	
	// thread options
	OS_STK *task_stack;
	int stack_size;
	int prio;
} datahub_options;

typedef struct datahub_client {
	char *instance_id;
	char *instance_key;
	char *user_name;
	char *client_id;
	datahub_options *options;
	datahub_connect_status status;
	// MQTT lib
	Network *n;
	Client c;
	void *message_group[DATAHUB_QUEUE_SIZE];
	OS_EVENT *message_queue;
	unsigned int current_reconnect_rate;
} datahub_client;

// create a datahub client instance
int datahub_create(datahub_client *client, char *instance_id, char *instance_key, 
					char *user_name, char *client_id, datahub_options *options);
// connect datahub server
int datahub_connect(datahub_client *client, Network *n);
// get sdk current connect status
datahub_connect_status datahub_isconnected(datahub_client *client);
int datahub_publish(datahub_client *client, char *topic, datahub_message *msg);
int datahub_subscribe(datahub_client *client, char *topic);
int datahub_unsubscribe(datahub_client *client, char *topic);
void datahub_disconnect(datahub_client *client);


#endif

