#ifndef MY_HSPHY_H
#define MY_HSPHY_H
#include "my_correct_types.h"
#include "IfxHsphy.h"


IfxHsphy_Geth_SgmiiSpeedConfigStatus myIfxHsphy_Geth_setSgmiiSpeedMode(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_TrgtDeviceSpeed trgtSpeed, IfxHsphy_GethXpcsParams *xpcs);

boolean myIfxHsphy_resetXpcs(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_GethXpcsParams *xpcs);


#endif
