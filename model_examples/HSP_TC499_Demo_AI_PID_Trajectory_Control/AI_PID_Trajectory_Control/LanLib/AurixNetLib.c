#include "AurixNetLib.h"

#ifdef MATLAB_MEX_FILE
#else
#include <string.h>
#include "led.h"
#include "serialio.h"

void AurixNetSetup(const uint8_t* ip, const uint8_t* netmask, const uint8_t* gw)
{
    Board_initLeds();
    Board_initSerialio();

    Ifx_Lwip_setup(ip, netmask, gw);
}

void AurixNetRec(uint8_t newData[1], uint8_t* data, uint16_t *size, uint16_t maxSize)
{
    Ifx_Lwip_pollTimers();
    Ifx_Lwip_pollRx();

    uint16 port;
    ip_addr_t ip;
    uint8_t* recData;
    uint16_t recSize;
    newData[0] = Ifx_Lwip_updateResultBuffer(&recData, &recSize, &ip, &port);
    if(newData[0] != 0) {
        size[0] = recSize > maxSize ? maxSize : recSize;
        memcpy(data, recData, size[0]);
    }
}

#if 0
void NetLink_cb(uint32_t param)
{
}
#endif

#endif
