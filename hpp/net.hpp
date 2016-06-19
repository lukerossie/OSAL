#ifndef NET_HPP
#define NET_HPP

void init_net();

struct tcp_socket;
tcp_socket *ctor_connect(char const *ip_address, int port);
//Do not use this function on a connected socket. Server sockets are never connected to a remote host.
// - SDL documentation
tcp_socket *ctor_accept();
void dtor_close(tcp_socket *sock);
void send(tcp_socket *who, char *data);
char *amem_recv(tcp_socket *who);

#endif