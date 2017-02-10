#ifndef DATAHUB_NETWORK_H
#define DATAHUB_NETWORK_H

typedef struct Network Network;

struct Network
{
   /*
    * A pointer pointing to what user passes using function NewNetwork(). It can be used in
    * function ConnectNetwork(), ucos_read(), ucos_write(), and ucos_disconnect()
    */
    void *data;

   /*
    * A function pointer. It will be used by SDK when reading data from network.
    * You should implement function ucos_read() to read data from network and pass it to variable 'read'
    */
    int (*read) (Network* Net, unsigned char* buffer, int len, int timeout_ms);

   /*
    * A function pointer. It will be used by SDK when writing data to network.
    * You should implement function ucos_write() to write data to network and pass it to variable 'write'
    */
    int (*write) (Network* Net, unsigned char* buffer, int len, int timeout_ms);

   /*
    * A function pointer. It will be used by SDK when disconnecting from network.
    * You should implement function ucos_disconnect() to disconnect from Network
    * and pass it to variable 'ucos_disconnect'
    */
    void (*disconnect) (Network* Net);

#define mqttread read
#define mqttwrite write
};

/*
 * describe: read data from network
 * parameter:
 *     Net: a pointer pointing to structure Network
 *     buffer: a buffer to store data reading from network
 *     len: length of buffer
 *     timeout_ms: ucos_read() must return after 'timeout_ms' milleseconds
 * ret:
 *     length of data reading successfully
 *
 */
int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/*
 * describe: write data to network
 * parameter:
 *     Net: a pointer pointing to structure Network
 *     buffer: a buffer storing data which will be write to network
 *     len: length of buffer
 *     timeout_ms: ucos_write() must return after 'timeout_ms' milleseconds
 * ret:
 *     length of data writing successfully
 *
 */
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);

/*
 * describe: disconnect from network
 * parameter:
 *     Net: a pointer pointing to structure Network
 * ret:
 *    none
 *
 */
void ucos_disconnect(Network* Net);

/*
 * describe: init structure 'Network'
 * parameter:
 *     Net: a pointer pointing to structure Network
 *     data: a pointer pointing to the structure user defines
 * ret:
 *    none
 *
 */
void NewNetwork(Network* Net, void *data);

/*
 * describe: connect to server
 * parameter:
 *     Net: a pointer pointing to structure Network
 *     addr: ip or domain name of server
 *     port: port of server
 * ret:
 *    0 means success. User can defined other error codes for own purpose
 *
 */

int ConnectNetwork(Network* Net, char* addr, int port);

#endif

