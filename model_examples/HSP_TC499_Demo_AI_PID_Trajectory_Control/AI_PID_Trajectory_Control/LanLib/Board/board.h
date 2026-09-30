#ifndef BOARD_H
#define BOARD_H

#include "Ifx_Cfg.h"

#ifdef BOARD_STD
#include "board_std.h"
#define PHY_HANDLE ethPhyRtl8201fi
#define BOARD_INIT_PHASE_1() Board_setupEthernetCtrl()
#define INIT_PHY(phyHandle) Board_initEthernetPhy(phyHandle)
#define INIT_PHASE_2(phyHandle) do{} while(0)
#elif defined BOARD_COM
#include "board_com.h"
#define PHY_HANDLE ethPhyRtl8221b
#define BOARD_INIT_PHASE_1() Board_Geth_Init_Phase1()
#define INIT_PHY(phyHandle) Board_RealtekPhy_Init(phyHandle)
#define INIT_PHASE_2(phyHandle) do{Board_RealtekPhy_Setup(phyHandle); Board_Geth_Init_Phase2();} while(0)

#endif

#endif
