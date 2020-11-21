#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32")

#define PORT 18363
#define PACKET_SIZE 1024
#define IP "---.---.---.---"

SOCKET hSocket;

char cMsg[PACKET_SIZE] = "";
char cBuffer[PACKET_SIZE] = "";

void proc_recv() {
    while (!WSAGetLastError()) {
        for (int i = 0; i < PACKET_SIZE; i++) {
            cBuffer[i] = '\0';
        }
        recv(hSocket, cBuffer, PACKET_SIZE, 0);
        printf("서버에서 데이터를 받았습니다: %s\n", cBuffer);

        if (cBuffer == "gameEnd") break;
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN tAddr = {};
    tAddr.sin_family = PF_INET;
    tAddr.sin_port = htons(PORT);
    inet_pton(PF_INET, IP, &(tAddr.sin_addr));

    connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
    std::thread proc1(proc_recv);

    while (1) {
        printf("\n============================\n");
        printf("> ");
        fgets(cMsg, PACKET_SIZE - 1, stdin);

        if (send(hSocket, cMsg, strlen(cMsg), 0) <= 0) { printf("send error\n"); break; }
        printf("데이터를 서버로 보냈습니다: %s\n\n", cMsg);
    } proc1.join();
    printf("bye while loop\n");
    closesocket(hSocket);
    WSACleanup();
    return 0;
}
