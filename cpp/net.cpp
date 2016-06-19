#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif

#include "../hpp/system.hpp"

struct tcp_socket
{
	TCPsocket sock;
}

constexpr int max_message_size_bytes=100;

tcp_socket *ctor_connect(char const *ip_address, int port)
{
	IPaddress myip;
	SDLNet_ResolveHost(&myip,ip_address,port);
	return SDLNet_TCP_Open(&myip);
}

void dtor_close(tcp_socket *sock)
{
	SDLNet_TCP_Close(sock.sock);
}

void send(tcp_socket *who, char *data)
{
	if(!who)return;
	if(!data.size())return;

	SDLNet_TCP_Send(who,data.c_str(),data.length());//should not need terminating null
	char const *error=SDLNet_GetError();
	if(error)
	{
		print("SDLNet send error.\n");
	}
}
char *amem_recv(tcp_socket *who)
{
	buff=(char *)amem(max_message_size_bytes+1);//null terminated
	for(int i=0; i<max_message_size_bytes+1; i++)
	{
		buff[i]=0;
	}
	auto bytes=SDLNet_TCP_Recv(who,buff,max_message_size_bytes);
	//buff[max_message_size_bytes]=0;
	if(bytes<=0)
	{
		return "";
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
