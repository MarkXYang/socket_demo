#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include "util.h"
#include "cobs.h"
#include "common.h"

void* msg_handler(void* param)
{
    char buf[BUF_SIZE];
    int count;
    pthread_detach(pthread_self());
    int s = (int)param;
    int len;
    uint8_t dst[BUF_SIZE];
    uint8_t buf_cobs[BUF_SIZE];

    while( count = read(s, buf, 1023) > 0 ) {
#ifdef DEBUG
        printf("Server received message\n");
#endif
        len = strlen(buf);

#ifdef DEBUG
        printf("Rcv %d bytes before uncobs:\n", len);
        printArray(buf, CODE_HEX, len);
#endif

        len = UnStuffData(buf, len, dst);
#ifdef DEBUG
        printf("After uncobs:\n");
        printArray(dst, CODE_HEX, len);
#endif

        if(verifyChkSum(dst, len)) { /* Checksum is wrong */
            dst[1] += 1;
            len = insertErrCode(dst, ERR_CHKSUM, len);
            len = calChkSum(dst, len-1);
            len = StuffData(dst, len, buf_cobs);
            write(s, buf_cobs, strlen(buf_cobs));
            close(s);
            break;
        } else if (dst[0] != ID_NAD) { /* Not the message to NAD */
            dst[1] += 1;
            len = insertErrCode(dst, ERR_GROUP_ID, len);
            len = calChkSum(dst, len-1);
            len = StuffData(dst, len, buf_cobs);
            write(s, buf_cobs, strlen(buf_cobs));
            close(s);
            break;
        } else {
            switch(dst[1])
            {
            case ECHO_ETH_REQ:
                dst[1] += 1;
                len = calChkSum(dst, len-1);
#ifdef DEBUG
                printf("After calChksum:\n");
                printArray(dst, CODE_HEX, len);
#endif

                len = StuffData(dst, len, buf_cobs);
#ifdef DEBUG
                printf("After cobs:\n");
                printArray(buf_cobs, CODE_HEX, len);
#endif
                write(s, buf_cobs, strlen(buf_cobs));
                break;

            default: /* The message id is out of the range */
                len = insertErrCode(dst, ERR_MESSAGE_ID, len);
                len = calChkSum(dst, len-1);
                len = StuffData(dst, len, buf_cobs);
                write(s, buf_cobs, strlen(buf_cobs));
                break;
            }
            close(s);
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno = PORT_ID;
    // create a TCP/IP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    struct sockaddr_in serv_addr;
    // clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    /* setup the host_addr structure for use in bind call */
    // server byte order
    serv_addr.sin_family = AF_INET;

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // port number to bind to
    serv_addr.sin_port = htons(portno);

    // This bind() call will bind  the socket to the current IP address on port
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    // This listen() call tells the socket to listen to the incoming connections.
    // The listen() function places all incoming connection into a backlog queue
    // until accept() call accepts the connection.
    // Here, we set the maximum size for the backlog queue to 5.
    listen(sockfd,5);

    while(newsockfd = accept(sockfd, 0, 0)) {
        pthread_t t;
        pthread_create(&t, 0, msg_handler, (void*)newsockfd);
    }

    return 0;
}
