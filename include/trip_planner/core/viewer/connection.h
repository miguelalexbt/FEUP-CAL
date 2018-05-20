#ifndef _CONNECTION_
#define _CONNECTION_

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#else
#include <WinSock2.h>
#endif

using namespace std;

class Connection {
public:
	Connection(short port);

	bool sendMsg(string msg);
	string readLine();
private:
#ifdef linux
	int sock;
#else
	SOCKET sock;
#endif
};

#endif