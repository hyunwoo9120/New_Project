#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
#pragma comment(lib, "ws2_32")
#define PACKET_SIZE 1024

#pragma once
#define ROOMCNT 5
using namespace std;

class RoomArray {
public:
	char cBuffer[PACKET_SIZE] = {}; // 게임 데이터
	char cMsg[PACKET_SIZE] = "GameStart"; // 가공된 데이터(유저한테 보낼 것)
	int userCnt;               // 유저 수
	SOCKET user[2];             // 유저 정보 소켓
	char recvData[PACKET_SIZE];
	char sendData[PACKET_SIZE];

	RoomArray() {
		userCnt = 0;
	}
	~RoomArray() {}

	void print() {
		for (int i = 0; i < ROOMCNT; i++) {
			cout << userCnt << " ";
		}
		cout << "\n";
	}

	int sendUsers() {
		int result;

		printf("데이터를 모든 유저에게 보냅니다: %s\n", cMsg);
		result = send(user[0], cMsg, strlen(cMsg), 0);
		if (result < 0)
			return -1;

		result = send(user[1], cMsg, strlen(cMsg), 0);
		if (result < 0)
			return -1;

		return 1;
	}

	int recvUser1() {
		int recvResult = recv(user[0], cBuffer, PACKET_SIZE, 0);
		//strcpy_s(cMsg, cBuffer);
		memcpy(cMsg, cBuffer, PACKET_SIZE);
		for (int i = 0; i < PACKET_SIZE; i++) {
			cBuffer[i] = '\0';
		}
		return recvResult;
	}
	int recvUser2() {
		int recvResult = recv(user[1], cBuffer, PACKET_SIZE, 0);
		//strcpy_s(cMsg, cBuffer);
		memcpy(cMsg, cBuffer, PACKET_SIZE);
		for (int i = 0; i < PACKET_SIZE; i++) {
			cBuffer[i] = '\0';
		}
		return recvResult;
	}

	void GameStart() {
		// TODO
		// Round turn에 따라서
		// 홀수 라운드: 수사관 잭 잭 수사관
		// 짝수 라운드: 잭 수사관 수사관 잭

		while (1) { // 하나의 클라이언트에서 반복해서 recv
			printf("\n============================\n");
			int sendResult = sendUsers();
			if (sendResult == -1) {
				printf("Send Err\n\n");
				closesocket(user[0]);
				closesocket(user[1]);
				break;
			}
			printf("(시작)두 유저에게 데이터를 보냈습니다.\n");
			while (1) {
				int recvResult = recvUser1();
				printf("\n유저1에게 데이터를 받았습니다: %s\n", cMsg);
				if (recvResult < 0) {
					printf("Recv Err\n\n");
					break;
				}
				else
				{
					int sendResult = sendUsers();
					if (sendResult == -1) {
						printf("Send Err\n\n");
						closesocket(user[0]);
						closesocket(user[1]);
						break;
					}
				}

				recvResult = recvUser2();
				printf("\n유저2에게 데이터를 받았습니다: %s\n", cMsg);
				if (recvResult < 0) {
					printf("Recv Err\n\n");
					break;
				}
				else
				{
					int sendResult = sendUsers();
					if (sendResult == -1) {
						printf("Send Err\n\n");
						closesocket(user[0]);
						closesocket(user[1]);
						break;
					}
				}

				//	for (int i = 0; i < PACKET_SIZE; i++) {
				//		cBuffer[i] = '\0';
				//	}
			}break;
		}
			
		printf("\n===== Client Disconnected =====\n");
		closesocket(user[0]);
		closesocket(user[1]);
	}
};
