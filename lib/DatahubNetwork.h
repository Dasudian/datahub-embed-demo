#ifndef DATAHUB_NETWORK_H
#define DATAHUB_NETWORK_H

typedef struct Network Network;

struct Network
{
/*
 * members
 */
    void *data;

/*
 * functions
 */
    int (*read) (Network* Net, unsigned char* buffer, int len, int timeout_ms);
    int (*write) (Network* Net, unsigned char* buffer, int len, int timeout_ms);
    void (*disconnect) (Network* Net);

#define mqttread read
#define mqttwrite write
};

int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);

void ucos_disconnect(Network* Net);

/*
 * describe: init structure 'Network'
 *
 */
void NewNetwork(Network* Net, void *data);
int ConnectNetwork(Network* Net, char* addr, int port);

#endif

