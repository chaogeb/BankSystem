#include <winsock2.h>
#include <mswsock.h>
#include <iostream>
#include "Facade.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

using namespace std;
using namespace CONTROLLER;

/*
服务器端编程的步骤：
1：加载套接字库，创建套接字(WSAStartup()/socket())；
2：绑定套接字到一个IP地址和一个端口上(bind())；
3：将套接字设置为监听模式等待连接请求(listen())；
4：请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字(accept())；
5：用返回的套接字和客户端进行通信(send()/recv())；
6：返回，等待另一连接请求；
7：关闭套接字，关闭加载的套接字库(closesocket()/WSACleanup())。
*/
bool FacadeController::Server()
{
	//server
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		throw std::runtime_error("ERROR");
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		throw std::runtime_error("ERROR");
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);

		char sendBuf[50];
		char recvBuf[50];
		char tempBuf[50];
		char *offset;
		char check[4];

		string cardNum;
		string psw;
		unsigned long long value;

		recv(sockConn, recvBuf, 50, 0);

		strncpy(check, recvBuf, 3);
		check[3] = '\0';
		if (atoi(check) == 0xff) {
			offset = recvBuf + 3;

			strncpy(check, offset, 2);		// cardNum len
			offset += 2;
			check[2] = '\0';
			int len = atoi(check);
			strncpy(tempBuf, offset, len);	// cardNum
			offset += len;
			tempBuf[len] = '\0';
			cardNum = tempBuf;

			strncpy(check, offset, 2);		// psw len
			offset += 2;
			check[2] = '\0';
			len = atoi(check);
			strncpy(tempBuf, offset, len);	// psw
			offset += len;
			tempBuf[len] = '\0';
			psw = tempBuf;

			value = atoll(offset);			// amount

			CardID cardID = GetCardID(cardNum.c_str());
			if (!FetchCard(cardNum.c_str())) {		// wrong card number
				sprintf(sendBuf, "%d", 0xfa);
				printf("wrong card number: %s\n", cardNum.c_str());
			}
			else if (VerifyCardSC(cardID, psw.c_str())) {	// password correct
				if (Withdraw(cardID, value)) {
					sprintf(sendBuf, "%d", 0xac);	// success
					printf("trade success: %s\n", cardNum.c_str());
				}
				else {
					sprintf(sendBuf, "%d", 0xfd);	// low balance
					printf("trade low balance: %s\n", cardNum.c_str());
				}
			}
			else {									// password incorrect
				sprintf(sendBuf, "%d", 0xfc);
				printf("password incorrect: %s\n", cardNum.c_str());
			}

			// return trade result
			send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		}
		else {
			sprintf(sendBuf, "%d", 0xff);
			send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		}

		//printf("%s\n", recvBuf);

		closesocket(sockConn);
	}
}