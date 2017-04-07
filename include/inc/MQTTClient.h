/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander/Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef __MQTT_CLIENT_C_
#define __MQTT_CLIENT_C_

#include "MQTTPacket.h"
#include "DatahubNetwork.h"
#include "DatahubTimer.h"

#define MAX_PACKET_ID 65535
#define MAX_MESSAGE_HANDLERS 1

enum QoS {
	QOS0 = 0,
	QOS1 = 1,
	QOS2 = 2
};

// all failure return codes must be negative
enum mqtt_error_code_s {
	MQTT_FAILURE = -1,
	MQTT_SUCCESS = 0
};

void NewTimer(Timer*);

typedef struct MQTTMessage MQTTMessage;
struct MQTTMessage
{
    enum QoS qos;
    char retained;
    char dup;
    unsigned short id;
    void *payload;
    size_t payloadlen;
};

typedef struct MessageData MessageData;
struct MessageData
{
    MQTTMessage* message;
    MQTTString* topicName;
};

typedef void (*messageHandler)(void *, MessageData*);

typedef struct Client Client;

/* timeout: ms */
int MQTTConnect (Client*, MQTTPacket_connectData*, int timeout_ms);
int MQTTPublish (Client* c, const char* topicName, MQTTMessage* message, unsigned int timeout_ms);
/* messageHandler can be NULL */
int MQTTSubscribe (Client*, const char*, enum QoS, int,messageHandler);
int MQTTUnsubscribe (Client*, const char*, int);
/* timeout: ms  */
int MQTTDisconnect (Client*, int timeout_ms);
int MQTTYield (Client*, int);

void setDefaultMessageHandler(Client*, messageHandler);

/* 
 * argument:
 *  command_timeout: ms
 *  connection_status_changed: can be NULL
 */

void MQTTClient(Client*, Network*, unsigned int, unsigned char*, size_t, unsigned char*, size_t, void *context,
        void (*connection_status_changed)(void *context, int isconnected));

struct Client {
    unsigned int next_packetid;
    unsigned int command_timeout_ms;
    size_t buf_size, readbuf_size;
    unsigned char *buf;  
    unsigned char *readbuf; 
    unsigned int keepAliveInterval;
    char ping_outstanding;
    int pingCount;// add by jack
    int isconnected;

    struct MessageHandlers
    {
        const char* topicFilter;
        void (*fp) (void*, MessageData*);
    } messageHandlers[MAX_MESSAGE_HANDLERS];      // Message handlers are indexed by subscription topic
    /* pass to callbacks */
    void *context;
    
    void (*defaultMessageHandler) (MessageData*);
    
    Network* ipstack;
    Timer ping_timer;

    void (*connection_status_changed)(void *context, int isconnected);
    /* 0 means we haven't notify that connection status has lost*/
    int isnotify_connection_lost;
};


#define DefaultClient {0, 0, 0, 0, NULL, NULL, 0, 0, 0}

#endif
