
#ifndef HEADER_SOCKEH
#define HEADER_SOCKEH
#include <thread>
#include<iostream>
#include<string>

#include<winsock2.h>
#include <WS2tcpip.h> 
#pragma comment(lib,"ws2_32.lib") 


extern bool FLag_Recv;
extern void appendMess(std::string s);


extern SOCKET listen_socket;

extern SOCKET remote_sock;


void StartServ(bool mode);

void RecvFromRemote(int id);



#endif // HEADER_SOCKEH