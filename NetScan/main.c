#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iphlpapi.h>
#include "dynstr.h"
#include <pcap.h>
#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "packet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

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

void CharToHex(char c, char* pOut /* 2 char wide */, int lowercase) {
	char hex[][31][1] = {
		{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", 0},
		{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", 0}
	};
	if (lowercase) {
		pOut[0] = hex[1][(c >> 4) & 0xf][0];
		pOut[1] = hex[1][c & 0xf][0];
	}
	else {
		pOut[0] = hex[0][(c >> 4) & 0xf][0];
		pOut[1] = hex[0][c & 0xf][0];
	}
}

int main(int argc, char** argv) {
	IPAddr dst = inet_addr("192.168.1.76");
	IPAddr src = inet_addr("192.168.1.76");

	CHAR MACAddrs[6];
	int len = 6;
	DWORD d = SendARP(dst, src, MACAddrs, &len);
	char buf[500];
	buf[0] = 0;
	for (int i = 0; i < len; i++) {
		char h[2];
		CharToHex(MACAddrs[i], h, 1);
		printf("%.*s%s", 2, h, (i + 1 == len) ? "" : ":");
	}
	printf("\n");
	struct pcap_if* allDevs;
	char errorBuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &allDevs, errorBuf) == -1)
		return 0x1;

	struct pcap_if* dev = allDevs;
	while (dev != NULL) {
		printf("%s --> %s\n", dev->name, dev->description);
		dev = dev->next;
	}

	return 0;
}