#include "DatahubNetwork.h"

int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);
void ucos_disconnect(Network* Net);

/*
 * 初始化Network结构体
 */
void NewNetwork(Network* Net, void *data)
{
	Net->data = data;
	Net->read = ucos_read;
	Net->write = ucos_write;
	Net->disconnect = ucos_disconnect;
}

int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms)
{
	//implement by user
}

int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms)
{
		//implement by user
}

void ucos_disconnect(Network* Net)
{
		//implement by user
}


int ConnectNetwork(Network* Net, char* addr, int port)
{
		//implement by user
}