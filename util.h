
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>


int calChkSum(uint8_t *ptr, uint8_t length);
int verifyChkSum(uint8_t *ptr, uint8_t length);
int insertErrCode(uint8_t *src, uint8_t errcode, int position, int len);
#if 0
uint8_t StuffData(const uint8_t *ptr, uint8_t length, uint8_t *dst);
uint8_t UnStuffData(const uint8_t *ptr, uint8_t length, uint8_t *dst);void error(const char *msg);
#endif

void error(const char *msg);

#ifdef DEBUG
enum codefmt {CODE_HEX, CODE_DEC, CODE_OCT, CODE_BIN};
void printArray(uint8_t *p, int code, int len);
#endif
#endif  // #ifndef _UTIL_H_
