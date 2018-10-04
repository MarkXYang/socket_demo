// client.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "util.h"
#include "cobs.h"
#include "common.h"

int main(int argc, char *argv[])
{
    int sockfd, portno = PORT_ID;
    struct sockaddr_in serv_addr;

	if(argc < 2) {
		printf("Usage: %s <ip address>\n", argv[0]);
		exit(-1);
	}
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
    {
        printf("Client send 'messange' to server %s:%d\n", argv[1], portno);
        uint8_t buf[BUF_SIZE];
        uint8_t cobs[BUF_SIZE];
        int len;
        buf[0] = ID_NAD;
        buf[1] = ECHO_ETH_REQ;
        strncpy(&buf[2], "AbCdEf", 6);
        len = calChkSum(buf, 8);
#ifdef DEBUG
        printf("checksum is:0x%X, len=%d\n", buf[len-1], len);
#endif
        if(verifyChkSum(buf, len)) {
            printf("There is error for checksum!\n");
			exit(-2);
        }
#ifdef DEBUG
        else {
            printf("Checksum is correct\n");
        }
#endif


        len = StuffData(buf, len, cobs);
        cobs[len] = 0;
#ifdef DEBUG
        printf("len before sending:%d\n", len);
        printArray(cobs, CODE_HEX, len);
#endif

        write(sockfd, cobs, strlen(cobs));

        memset(buf, 0, BUF_SIZE*sizeof(uint8_t));
        memset(cobs, 0, BUF_SIZE*sizeof(uint8_t));

        int count = read(sockfd, cobs, BUF_SIZE);

#ifdef DEBUG
        printArray(cobs, CODE_HEX, count);
#endif

        len = UnStuffData(cobs, count, buf);
#ifdef DEBUG
        printArray(buf, CODE_HEX, len);
#endif

        if(verifyChkSum(buf, len)) {
            printf("There is error for checksum!\n");
        }
        else {
            if (buf[2]) {
                printf("Error with errcode = %d\n", buf[2]);
            } else {
                buf[len-1] = 0;
                printf("Got the result from groupID-%d is: %s, len is %d\n",
						buf[0], &buf[3], len-4);
            }
        }

        memset(buf, 0, BUF_SIZE*sizeof(uint8_t));
        memset(cobs, 0, BUF_SIZE*sizeof(uint8_t));

        shutdown(sockfd, SHUT_RDWR);
    }
    else
        error("ERROR connecting");
}
