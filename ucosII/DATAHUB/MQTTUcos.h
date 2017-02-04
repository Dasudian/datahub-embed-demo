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
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef __MQTT_UCOS_
#define __MQTT_UCOS_

#include "lwip/api.h"

typedef struct Timer Timer;

struct Timer {
	unsigned int end_time;
};

typedef struct Network Network;

struct Network
{
	int my_socket;
	//struct netconn *netconn;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};

void InitTimer(Timer*);
char expired(Timer*);
void countdown_ms(Timer*, unsigned int);
void countdown(Timer*, unsigned int);
int left_ms(Timer*);


int ucos_read(Network*, unsigned char*, int, int);
int ucos_write(Network*, unsigned char*, int, int);
void ucos_disconnect(Network*);
void NewNetwork(Network*);

int ConnectNetwork(Network*, char*, int);

#endif

