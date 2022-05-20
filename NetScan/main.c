#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 80

// The IP header
struct IPHeader {
    BYTE h_len : 4;           // Length of the header in dwords
    BYTE version : 4;         // Version of IP
    BYTE tos;               // Type of service
    USHORT total_len;       // Length of the packet in dwords
    USHORT ident;           // unique identifier
    USHORT flags;           // Flags
    BYTE ttl;               // Time to live
    BYTE proto;             // Protocol number (TCP, UDP etc)
    USHORT checksum;        // IP checksum
    ULONG source_ip;
    ULONG dest_ip;
};

// ICMP header
struct ICMPHeader {
    BYTE type;          // ICMP packet type
    BYTE code;          // Type sub code
    USHORT checksum;
    USHORT id;
    USHORT seq;
    ULONG timestamp;    // not part of ICMP, but we need it
};

int main(int argc, char** argv) {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sfd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 0, 0);
    if (sfd != INVALID_SOCKET) {
        printf("VALID SOCKET\n");
    }
    {
        DWORD EnableKey = TRUE;
        setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &EnableKey, sizeof(DWORD));
        printf("%s\n", (HRESULT_FROM_WIN32(WSAGetLastError()) == S_OK) ? "OKAY" : "NOT OKAY");
        INT ttl = 50;
        setsockopt(sfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(INT));
        printf("%s\n", (HRESULT_FROM_WIN32(WSAGetLastError()) == S_OK) ? "OKAY" : "NOT OKAY");
    }

    int i = 5;
    while (i > 0) {
        char buffer[2048];
        ZeroMemory(buffer, 2048);
        SOCKADDR_IN fromAddr = {
            .sin_family = AF_INET,
            .sin_port = PORT
        };
        inet_pton(AF_INET, "192.168.1.76", &fromAddr.sin_addr);
        int ret = recvfrom(sfd, buffer, sizeof(buffer), 0, (SOCKADDR*)&fromAddr, sizeof(SOCKADDR_IN));
        if (ret == -1) {
            printf("Error reciving data.");
            HRESULT hr = HRESULT_FROM_WIN32(WSAGetLastError());
            break;
        }
        if (ret > 0) {
            printf("[%i]: [%.*s]\n", i, 512, buffer);
            i--;
        }
    }

    closesocket(sfd);

    WSACleanup();
    return 0;
}