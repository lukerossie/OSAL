#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../h/util.h"

typedef struct tcp_socket
{
	TCPsocket sock;
} tcp_socket;

const int MAX_MESSAGE_SIZE_BYTES=100;

tcp_socket *ctor_tcp_socket_connect(char const *ip_address, int port)
{
	IPaddress myip;
	tcp_socket *sock;

	SDLNet_ResolveHost(&myip,ip_address,port);
	sock=(tcp_socket*)malloc(sizeof(tcp_socket));
	sock->sock=SDLNet_TCP_Open(&myip);
	return sock;
}

void dtor_tcp_socket_close(tcp_socket *sock)
{
	SDLNet_TCP_Close(sock->sock);
	free(sock);
}

void send(tcp_socket *who, char *data)
{
	char const *error;
	if(!who)return;
	if(!data||!data[0])return;

	SDLNet_TCP_Send(who->sock,data,strlen(data));/*should not need terminating null*/
	error=SDLNet_GetError();
	if(error)
	{
		printf("SDLNet send error.\n");
	}
}
char *malloc_recv(tcp_socket *who)
{
	char *buff=(char*)malloc(MAX_MESSAGE_SIZE_BYTES+1);/*null terminated*/
	int i=0;
	u32 bytes;
	for(i=0; i<MAX_MESSAGE_SIZE_BYTES+1; i++)
	{
		buff[i]=0;
	}
	bytes=SDLNet_TCP_Recv(who->sock,buff,MAX_MESSAGE_SIZE_BYTES);
	
	if(bytes<=0)
	{
		return NULL;
	}
	return buff;
}

void init_net()
{
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init error.\n");
	}
}
