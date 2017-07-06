#include "DatahubNetwork.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "datahub_demo.h"

int ucos_read(Network* Net, unsigned char* buffer, int len, int timeout_ms);
int ucos_write(Network* Net, unsigned char* buffer, int len, int timeout_ms);
void ucos_disconnect(Network* Net);

int errno;
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

/**
读取多少个字节的数据
buffer：用来接收读取到的数据
len：想要读取多少数据
timeout_ms：读取数据在多少毫秒后超时
*/
int ucos_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    int bytes = 0;
    struct self_s *self = (struct self_s *)n->data;

    setsockopt(self->socket, SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout_ms, sizeof(int));

    while (bytes < len)
    {
        int rc = recv(self->socket, &buffer[bytes], (size_t)(len - bytes), 0);
        if (rc > 0)
        {
            bytes += rc;
        }
        else if (rc == 0) {
            printf("reset by server\n");
            return -1;
        }
        else {
            if (errno != EAGAIN && errno != ECONNRESET) {
                printf("recv() failed, errno [ %d ]\n", errno);
            }
            return -1;
        }
    }
    return bytes;
}

int ucos_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    int rc;
    struct self_s *self = (struct self_s *)n->data;

    setsockopt(self->socket, SOL_SOCKET, SO_SNDTIMEO, (void*)&timeout_ms, sizeof(int));
    rc = write(self->socket, buffer, len);
    if (rc == len) {
        printf("write() success, len [ %d ]\n", rc);
    }else {
        printf("write() failed, rc [ %d ] errno [%d]\n", rc, errno);
        return -1;
    }
    return rc;
}

void ucos_disconnect(Network* n)
{
    struct self_s *self = (struct self_s *)n->data;

    close(self->socket);

}



int ConnectNetwork(Network* n, char* addr, int port)
{
    int type = SOCK_STREAM;
    struct sockaddr_in address ={0};
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
    } else {
        printf("getaddrinfo() failed, rc [ %d ] errno [%d]\n", rc, errno);
        return -1;
    }

    self->socket = socket(family, type, 0);
    if ( -1 == self->socket)
    {
        printf("socket() failed, rc [ %d ] errno [%d]\n", rc, errno);
        return -1;  
    }
    
    rc = connect(self->socket, (struct sockaddr*)&address, sizeof(address));
    if ( -1 == rc)
    {
        printf("connect() failed, rc [ %d ] errno [%d]\n", rc, errno);
        close(self->socket);
        return -1;  
    }
    
    return 0;
}
