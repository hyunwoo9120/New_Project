#include "./RoomArray.h"
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

#define PORT 18363
#define PACKET_SIZE 1024

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET hListen;
    SOCKET hClient;
    hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN tListenAddr = {};
    tListenAddr.sin_family = PF_INET;
    tListenAddr.sin_port = htons(PORT);
    tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
    listen(hListen, SOMAXCONN);

    // 여기부터 데이터 송수신

    SOCKADDR_IN tClntAddr = {};

    // 룸 배열 생성
    RoomArray roomList[ROOMCNT];

    while (1) { // 여러 클라이언트 접속허용
        printf("Waiting...\n");

        int iClntSize = sizeof(tClntAddr);
        hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

        // accept 오류확인
        if (hClient == -1) {
            closesocket(hClient);
            printf("accept error\n");
            break;
        }

        // 클라이언트 연결이 확인됨.
        else printf("\n----- Connection Established -----\n");
        // roomList room의 인원수를 확인. 이론상 인원수가 2일리는 없음
        for (int i = 0; i < ROOMCNT; i++) {
            switch (roomList[i].userCnt) {
            case 0:
                roomList[i].userCnt++;
                roomList[i].user[0] = hClient;
                break;
            case 1:
                roomList[i].userCnt++;
                roomList[i].user[1] = hClient;
                // 현재 방 하나에 대해서만 작동
                // TODO: 스레드로 작업
                roomList[i].GameStart();
                break;
            default: break;
            }
        }
    } // 전체 while 종료
    closesocket(hListen);

    WSACleanup();
    return 0;
}
