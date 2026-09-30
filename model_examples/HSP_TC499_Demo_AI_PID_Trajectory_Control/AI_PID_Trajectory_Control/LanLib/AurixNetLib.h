#ifndef AURIXNETLIB_H_
#define AURIXNETLIB_H_

#include <stdint.h>

#ifdef MATLAB_MEX_FILE
static inline void AurixNetSetup(const uint8_t* ip, const uint8_t* netmask, const uint8_t* gw) { }
static inline void AurixNetSend(const uint8_t* data, uint16_t size) { }
static inline void AurixNetRec(uint8_t newData[1], uint8_t* data, uint16_t *size, uint16_t maxSize) { }

#else
#include "UDP_Setup_LWIP.h"

void AurixNetSetup(const uint8_t* ip, const uint8_t* netmask, const uint8_t* gw);
void AurixNetRec(uint8_t newData[1], uint8_t* data, uint16_t *size, uint16_t maxSize);

static inline void AurixNetSend(const uint8_t* data, uint16_t size)
{
    Ifx_Lwip_send_back(data, size);
}

#endif

#endif
