
/*
 *
 */
#include "util.h"
#include <stdlib.h>
#include <stdio.h>


/*
 *
 */
int calChkSum(uint8_t *ptr, uint8_t length)
{
    char i;
    uint8_t cs = 0;
    for (i=0; i<length; i++) {
        cs += ptr[i];
    }
    ptr[length] = cs;
#ifdef DEBUG
    printf("checksum:0x%X, len:%d\n", cs, length);
#endif
    return length+1;
}

/*
 *
 */
int verifyChkSum(uint8_t *ptr, uint8_t length)
{
    char i;
    uint8_t cs = 0;
    for (i=0; i<length-1; i++) {
        cs += ptr[i];
    }

#ifdef DEBUG
    printf("checksum:0x%X, len:%d\n", cs, length);
    printf("origin:0x%X, cal:0x%X, len:%d\n", ptr[length-1], cs, length);
#endif
    if (ptr[length-1] != cs)
        return -1;
    else
        return 0;
}

/*
 * Insert the errocode to the beginning of the buffer for response message
 */
int insertErrCode(uint8_t *src, uint8_t errcode, int len)
{
    for(int i=len; i>0; i--)
        src[i] = src[i-1];
    src[0] = errcode;
    return len+1;
}

/*
 *
 */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

#ifdef DEBUG

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c "
#define BYTE_TO_BINARY(byte) \
	(byte & 0x80 ? '1' : '0'), \
	(byte & 0x40 ? '1' : '0'), \
	(byte & 0x20 ? '1' : '0'), \
	(byte & 0x10 ? '1' : '0'), \
	(byte & 0x08 ? '1' : '0'), \
	(byte & 0x04 ? '1' : '0'), \
	(byte & 0x02 ? '1' : '0'), \
	(byte & 0x01 ? '1' : '0')

void printArray(uint8_t *p, int code, int len)
{
    printf("Array size=%d\n", len);
    for(int j=0; j<len; j++) {
        switch (code) {
        case CODE_HEX:
            printf("0x%X ", p[j]);
            break;
        case CODE_DEC:
            printf("%d ", p[j]);
            break;
        case CODE_OCT:
            printf("%o ", p[j]);
            break;
        case CODE_BIN:
            printf(BYTE_TO_BINARY_PATTERN,  BYTE_TO_BINARY(p[j]));
            break;
        default:
            printf("%d ", p[j]);
            break;
        }
    }
    printf("\n");
}



#endif



