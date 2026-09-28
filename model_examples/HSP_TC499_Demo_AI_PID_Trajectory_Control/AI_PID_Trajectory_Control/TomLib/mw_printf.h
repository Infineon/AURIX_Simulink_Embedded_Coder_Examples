#ifndef MW_PRINTF_H_
#define MW_PRINTF_H_

// To support "PRId64", we need to include as follows
#include <inttypes.h>

#if defined(STM32)
//#include "main.h"
//extern UART_HandleTypeDef huart2;
#define MW_PRINTF(...)	do {	static char _buf[64]; \
								int len = sprintf(_buf, __VA_ARGS__); \
								HAL_UART_Transmit(&huart2, (uint8_t*)_buf, len, 1000); } while(0);

#elif defined(__TASKING__)
#include <stdio.h>
#define MW_PRINTF(...)	        printf(__VA_ARGS__)

#else   // Any other type
#include <stdio.h>
#define MW_PRINTF(...)	        do { printf(__VA_ARGS__); fflush(stdout); } while(0)
#define PIL_PRINTF(...)	        MW_PRINTF(__VA_ARGS__)
#endif

// Debugging macros, printing code line, variable name and its content
#define MAKE_STR_(s)       #s
#define MAKE_STR(x)        MAKE_STR_(x)

// Debug print integer variable
#define DBGI(x)  MW_PRINTF("L:%d " MAKE_STR(x) ": %d\n", __LINE__, x)
// Floating point variable
#define DBGD(x)  MW_PRINTF("L:%d " MAKE_STR(x) ": %f\n", __LINE__, x)
// String variable
#define DBGS(x)  MW_PRINTF("L:%d " MAKE_STR(x) ": %s\n", __LINE__, x)
// Text as literal
#define DBGT(x)  MW_PRINTF("L:%d %s\n", __LINE__, x)

#endif 	// MW_PRINTF_H_
