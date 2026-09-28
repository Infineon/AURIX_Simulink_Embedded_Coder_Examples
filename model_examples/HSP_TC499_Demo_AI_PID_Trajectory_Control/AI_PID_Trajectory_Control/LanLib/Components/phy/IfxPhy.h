#ifndef IFX_PHY_H
#define IFX_PHY_H

#include "Ifx_Cfg.h"
#ifdef BOARD_STD
#include "IfxGeth_Phy_Rtl8201fi.h"
#define PHY_IS_LINK(phyHandle,linkState) IfxGeth_Eth_Phy_Rtl8201fi_getLinkState(phyHandle,linkState)
#elif defined BOARD_COM
#include "rtl8226b.h"
#define PHY_IS_LINK(phyHandle,linkState) Rtl8226b_is_link(phyHandle, linkState)
#endif

#endif
