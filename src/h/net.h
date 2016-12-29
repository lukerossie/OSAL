#ifndef NET_H
#define NET_H

void init_net();

struct tcp_socket;
tcp_socket *ctor_tcp_socket_connect(char const *ip_address, int port);
/*
Do not use this function on a connected socket. Server sockets are never connected to a remote host.
 - SDL documentation
*/
tcp_socket *ctor_tcp_socket_accept();
void dtor_tcp_socket_close(tcp_socket *sock);
void send(tcp_socket *who, char *data);
char *malloc_recv(tcp_socket *who);/*returns NULL if recv 0 bytes*/

#endif