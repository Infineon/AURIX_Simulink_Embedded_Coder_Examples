#include "my_correct_types.h"
#include "myHsphy.h"
#include "IfxHsphy.h"

boolean myIfxHsphy_resetXpcs(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_GethXpcsParams *xpcs)
{
    uint32 timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;
    uint8  timeOutError      = 0U;
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].MII.DIG_CTRL1.B.VR_RST = 0;

    hsphyRegPtr->XPCS[xpcs->xpcsIndex].MII.DIG_CTRL1.B.VR_RST = 1;

    while (hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_SRAM.B.INIT_DN != 1)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return TRUE;
    }

    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_SRAM.B.EXT_LD_DN = 1;

    timeoutCycleCount                                                            = IFXHSPHY_MAX_TIMEOUT;

    while (hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.SR_XS_CTRL1.B.RST == 1)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    return (boolean)timeOutError;
}

boolean myIfxHsphy_sgmiiMpllDisable(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_XpcsIndex xpcsIndex)
{
    uint32 timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;
    uint8  timeOutError      = 0U;
    //Disable MPLL
    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_MPLL_CMN_CTRL.B.MPLL_EN_0 = 0;
    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_TX_GENCTRL2.B.TX_REQ_0        = 1;

    while (1 == hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_TX_GENCTRL2.B.TX_REQ_0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return TRUE;
    }

    timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;

    while (1 == hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_STS.B.TX_ACK_0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    return (boolean)timeOutError;
}


boolean myIfxHsphy_sgmiiMpllEnable(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_XpcsIndex xpcsIndex)
{
    uint32 timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;
    uint8  timeOutError      = 0U;
    //Enable/Bring-up MPLL in 1.25Gb/s rate
    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_MPLL_CMN_CTRL.B.MPLL_EN_0 = 1;
    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_TX_GENCTRL2.B.TX_REQ_0        = 1;

    while (1 == hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_TX_GENCTRL2.B.TX_REQ_0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return TRUE;
    }

    timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;

    while (1 == hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_STS.B.TX_ACK_0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return TRUE;
    }

    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_RX_GENCTRL2.B.RX_REQ_0 = 1;

    timeoutCycleCount                                                        = IFXHSPHY_MAX_TIMEOUT;

    while (1 == hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_RX_GENCTRL2.B.RX_REQ_0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return TRUE;
    }

    timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;

    while (1 == hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_RX_STS.B.RX_ACK_0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    return (boolean)timeOutError;
}

boolean myIfxHsphy_sgmiiXpcsDataPathInit(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_XpcsIndex xpcsIndex)
{
    uint32 timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;
    uint8  timeOutError      = 0U;
    //Initialize XPCS data path
    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_GENCTRL1.B.TX_CLK_RDY_0 = 0;
    hsphyRegPtr->XPCS[xpcsIndex].PCS.SR_XS_CTRL1.B.LPM                               = 1; //power down the DWC_xpcs

    while (hsphyRegPtr->XPCS[xpcsIndex].PCS.VR_XS_DIG_STS.B.PSEQ_STATE != IfxHsphy_XpcsPowerUpSeqState_powerDown)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return TRUE;
    }

    hsphyRegPtr->XPCS[xpcsIndex].PCS.SR_XS_CTRL1.U &= (uint32)(~((1 << IFX_HSPHY_XPCS_PCS_SR_XS_CTRL1_LPM_OFF) | (1 << IFX_HSPHY_XPCS_PCS_SR_XS_CTRL1_RST_OFF)));      //power up the DWC_xpcs

    timeoutCycleCount                               = IFXHSPHY_MAX_TIMEOUT;

    while (hsphyRegPtr->XPCS[xpcsIndex].PCS.VR_XS_DIG_STS.B.PSEQ_STATE != IfxHsphy_XpcsPowerUpSeqState_powerGood)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    hsphyRegPtr->XPCS[xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_GENCTRL1.B.TX_CLK_RDY_0 = 1;
    return (boolean)timeOutError;
}

IfxHsphy_Geth_SgmiiSpeedConfigStatus myIfxHsphy_Geth_setSgmiiSpeedMode(Ifx_HSPHY *hsphyRegPtr, IfxHsphy_TrgtDeviceSpeed trgtSpeed, IfxHsphy_GethXpcsParams *xpcs)
{
    uint32              timeoutCycleCount = IFXHSPHY_MAX_TIMEOUT;
    uint8               timeOutError      = 0U;
    IfxHsphy_SgmiiSpeed sgmiiSpeed        = IfxHsphy_SgmiiSpeed_1G;
    boolean             relvalCheck = FALSE;

    relvalCheck = myIfxHsphy_sgmiiMpllDisable(hsphyRegPtr, xpcs->xpcsIndex);
    /* volatile uint32* addr = (uint32*) 0xF28601C0u; */
    /* *addr &= 0xFFFFFFFE; */

    if (relvalCheck == TRUE)
    {
        return IfxHsphy_Geth_SgmiiSpeedConfigStatus_timeOutError;
    }

    if (trgtSpeed == IfxHsphy_TrgtDeviceSpeed_0P1G)
    {
        //Set XPCS in 100M mode
        sgmiiSpeed                                                        = IfxHsphy_SgmiiSpeed_0P1G;
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.SR_XS_CTRL2.B.PCS_TYPE_SEL = IFXHSPHY_SET_FIELD_VALUE(XPCS_PCS_SR_XS_CTRL2, PCS_TYPE_SEL, IfxHsphy_PcsTypeSel_10GBASE_X);
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].MII.DIG_CTRL1.B.EN_100M        = 1;
    }
    else if (trgtSpeed == IfxHsphy_TrgtDeviceSpeed_1G)
    {
        //Set XPCS in 1G mode
        sgmiiSpeed                                                        = IfxHsphy_SgmiiSpeed_1G;
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.SR_XS_CTRL2.B.PCS_TYPE_SEL = IFXHSPHY_SET_FIELD_VALUE(XPCS_PCS_SR_XS_CTRL2, PCS_TYPE_SEL, IfxHsphy_PcsTypeSel_10GBASE_X);
    }

    else if (trgtSpeed == IfxHsphy_TrgtDeviceSpeed_2P5G)
    {
        //Set XPCS in 2.5G mode
        sgmiiSpeed                                                            = IfxHsphy_SgmiiSpeed_2P5G;
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.SR_XS_CTRL2.B.PCS_TYPE_SEL     = IFXHSPHY_SET_FIELD_VALUE(XPCS_PCS_SR_XS_CTRL2, PCS_TYPE_SEL, IfxHsphy_PcsTypeSel_10GBASE_X);
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.VR_XS_DIG_CTRL1.B.EN_2_5G_MODE = 1;
    }

    else if (trgtSpeed == IfxHsphy_TrgtDeviceSpeed_5G)
    {
        //Set XPCS in 5G mode
        sgmiiSpeed                                                        = IfxHsphy_SgmiiSpeed_5G;
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.SR_XS_CTRL2.B.PCS_TYPE_SEL = IFXHSPHY_SET_FIELD_VALUE(XPCS_PCS_SR_XS_CTRL2, PCS_TYPE_SEL, IfxHsphy_PcsTypeSel_2P5GBASE_X);
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.SR_CTRL1.B.SS13            = 0;
    }
    else
    {
        return IfxHsphy_Geth_SgmiiSpeedConfigStatus_invalidConfigError;
    }

    //config the ref freq selected
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_REF_CLK_CTRL.B.REF_MPLLA_DIV2 = IfxHsphy_Sgmii_refClkConfig[xpcs->xpcsRefClk][IfxHsphy_SgmiiRefClkBits_refMpllaDiv2];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_REF_CLK_CTRL.B.REF_CLK_DIV2   = IfxHsphy_Sgmii_refClkConfig[xpcs->xpcsRefClk][IfxHsphy_SgmiiRefClkBits_refClKDiv2];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_REF_CLK_CTRL.B.REF_RANGE      = IfxHsphy_Sgmii_refClkConfig[xpcs->xpcsRefClk][IfxHsphy_SgmiiRefClkBits_refRange];

    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.VR_XS_DIG_CTRL1.B.VR_RST                           = 1;

    while (hsphyRegPtr->XPCS[xpcs->xpcsIndex].PCS.VR_XS_DIG_CTRL1.B.VR_RST != 0)
    {
        IFXHSPHY_LOOP_TIMEOUT_CHECK(timeoutCycleCount, timeOutError);
    }

    if (timeOutError == TRUE)
    {
        return IfxHsphy_Geth_SgmiiSpeedConfigStatus_timeOutError;
    }

    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_MPLLA_CTRL0.B.MPLLA_MULTIPLIER        = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_mpllaMultiplier];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_MPLLA_CTRL2.B.MPLLA_TX_CLK_DIV        = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_mpllaTxClkDiv];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_MPLLA_CTRL2.B.MPLLA_DIV16P5_CLK_EN    = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_mpllaDiv16P5ClkEnable];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_8G_MPLLA_CTRL6.B.CP_PROP                      = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_cpProp];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_8G_MPLLA_CTRL7.B.CP_PROP_GS                   = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_cpPropGs];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_16G_MPLLA_CTRL1.B.MPLLA_FRACN_CTRL            = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_mpllaFrancCtrl];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_VCO_CAL_LD0.B.VCO_LD_VAL_0        = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_vcoLdVal_0];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_VCO_CAL_REF0.B.VCO_REF_LD_0               = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_vcoRefLd_0];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_16G_25G_MISC_CTRL2.B.SUP_MISC                 = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_supMisc];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_RX_GENCTRL1.B.RX_DIV16P5_CLK_EN_0 = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_rxDiv16P5ClkEn_0];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_16G_25G_RX_GENCTRL4.B.RX_125MHZ_CLK_EN_0      = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_rx125MhzClkEn_0];

    if (trgtSpeed == IfxHsphy_TrgtDeviceSpeed_0P1G)
    {
        hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_RX_CDR_CTRL.B.CDR_TRACK_EN_0 = 0;
    }

    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_RATE_CTRL.B.TX0_RATE    = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_tx0Rate];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_RX_RATE_CTRL.B.RX0_RATE    = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_rx0Rate];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_16G_25G_RX_MISC_CTRL0.B.RX0_MISC       = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_rx0Misc];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_TX_GENCTRL2.B.TX0_WIDTH        = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_tx0Width];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_RX_GENCTRL2.B.RX0_WIDTH        = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_rx0Width];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_BOOST_CTRL.B.TX0_IBOOST = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_tx0Iboost];
    hsphyRegPtr->XPCS[xpcs->xpcsIndex].PMA.VR_XS_MP_12G_16G_25G_TX_GENCTRL1.B.VBOOST_EN_0  = IfxHsphy_Sgmii_paramConfig[sgmiiSpeed][xpcs->xpcsRefClk][IfxHsphy_SgmiiParamIndex_vBoostEn_0];

    relvalCheck                                                                            = myIfxHsphy_sgmiiMpllEnable(hsphyRegPtr, xpcs->xpcsIndex);

    if (relvalCheck == TRUE)
    {
        return IfxHsphy_Geth_SgmiiSpeedConfigStatus_timeOutError;
    }

    relvalCheck = myIfxHsphy_sgmiiXpcsDataPathInit(hsphyRegPtr, xpcs->xpcsIndex);

    if (relvalCheck == TRUE)
    {
        return IfxHsphy_Geth_SgmiiSpeedConfigStatus_timeOutError;
    }

    return IfxHsphy_Geth_SgmiiSpeedConfigStatus_success;
}
