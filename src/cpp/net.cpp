#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif

#include "../hpp/system.hpp"
#include "../hpp/util.hpp"

struct tcp_socket
{
	TCPsocket sock;
};

constexpr int max_message_size_bytes=100;

tcp_socket *ctor_tcp_socket_connect(char const *ip_address, int port)
{
	IPaddress myip;
	SDLNet_ResolveHost(&myip,ip_address,port);
	tcp_socket *sock=new tcp_socket();
	sock->sock=SDLNet_TCP_Open(&myip);
	return sock;
}

void dtor_tcp_socket_close(tcp_socket *sock)
{
	SDLNet_TCP_Close(sock->sock);
	delete sock;
}

void send(tcp_socket *who, char *data)
{
	if(!who)return;
	if(!data||!data[0])return;

	SDLNet_TCP_Send(who->sock,data,cstr_len(data));//should not need terminating null
	char const *error=SDLNet_GetError();
	if(error)
	{
		print("SDLNet send error.\n");
	}
}
char *new_recv(tcp_socket *who)
{
	char *buff=new char[max_message_size_bytes+1];//null terminated
	for(int i=0; i<max_message_size_bytes+1; i++)
	{
		buff[i]=0;
	}
	auto bytes=SDLNet_TCP_Recv(who->sock,buff,max_message_size_bytes);
	//buff[max_message_size_bytes]=0;
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
		print("SDLNet_Init error.\n");
	}
}
