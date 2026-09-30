/**
 * \file  SmmReset.c
 * \brief Standalone software-reset trigger for TC49x (no iLLD dependency).
 *
 * All register addresses and bit-field offsets are taken directly from
 * the TC49x User Manual / SFR headers and are hard-coded here so that
 * this file compiles without any iLLD include path.
 *
 * Register sources (IfxSmm_reg.h / IfxSmm_bf.h / IfxProt_bf.h):
 *   SMM_PROTE       @ 0xF006000C  – Protection Enable register
 *   SMM_RSTTRIGCTRLA@ 0xF0060114  – Reset Trigger Control A register
 *   SMM_SWRSTCON    @ 0xF00600C0  – Software Reset Control register
 */

#include "SmmReset.h"

#define RESET_ENABLED 1
/* -------------------------------------------------------------------------
 * TC49x SMM SFR addresses
 * ------------------------------------------------------------------------- */

/** SMM Protection Enable register */
#define SMM_PROTE_REG            (*(volatile uint32_t *)0xF006000CUL)

/** SMM Reset Trigger Control A register */
#define SMM_RSTTRIGCTRLA_REG     (*(volatile uint32_t *)0xF0060114UL)

/** SMM Software Reset Control register */
#define SMM_SWRSTCON_REG         (*(volatile uint32_t *)0xF00600C0UL)

/* -------------------------------------------------------------------------
 * PROTE bit-field constants  (IfxProt_bf.h: STATE[2:0] @ bit 0, SWEN @ bit 3)
 * ------------------------------------------------------------------------- */
#define PROT_STATE_OFF           (0U)
#define PROT_STATE_MSK           (0x7U)
#define PROT_SWEN_OFF            (3U)
#define PROT_SWEN_MSK            (0x1U)

/** PROT state value for "config" mode (IfxApProt_State_config = 1) */
#define PROT_STATE_CONFIG        (1U)

/* -------------------------------------------------------------------------
 * RSTTRIGCTRLA bit-field constants  (IfxSmm_bf.h: SW[2:0] @ bit 24)
 * ------------------------------------------------------------------------- */
#define RSTTRIGCTRLA_SW_OFF      (24U)
#define RSTTRIGCTRLA_SW_MSK      (0x7U)

/* -------------------------------------------------------------------------
 * SWRSTCON bit-field constants  (IfxSmm_bf.h: SWRSTREQ @ bit 1)
 * ------------------------------------------------------------------------- */
#define SWRSTCON_SWRSTREQ_OFF    (1U)
#define SWRSTCON_SWRSTREQ_MSK    (0x1U)

/* -------------------------------------------------------------------------
 * Spin-loop count that gives hardware enough time to assert the reset.
 * Mirrors IFXSMMRST_PERFORM_RESET_DELAY = 90000 from IfxSmmRst.h.
 * ------------------------------------------------------------------------- */
#define SMM_RESET_DELAY          (90000U)

/* -------------------------------------------------------------------------
 * Internal helper: unlock the SMM PROTE register for write access.
 *
 * Mirrors what IfxApProt_setState() does:
 *   prot.B.SWEN  = 1;
 *   prot.B.STATE = state;
 *   protReg->U   = prot.U;
 * ------------------------------------------------------------------------- */
static void smm_unlockProte(void)
{
    uint32_t reg = SMM_PROTE_REG;

    /* Clear STATE[2:0] and SWEN, then set STATE=config(1) and SWEN=1 */
    reg &= ~((PROT_STATE_MSK << PROT_STATE_OFF) | (PROT_SWEN_MSK << PROT_SWEN_OFF));
    reg |=  ((PROT_STATE_CONFIG & PROT_STATE_MSK) << PROT_STATE_OFF)
          | (1U << PROT_SWEN_OFF);

    SMM_PROTE_REG = reg;
}

/* -------------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------------- */
void SmmReset_triggerReset(int enable)
{
    if (RESET_ENABLED == enable){
        uint32_t i;
        uint32_t reg;
        SmmReset_Type rstType = SmmReset_Type_systemReset;
        /* Step 1: unlock SMM registers (PROTE → config state) */
        smm_unlockProte();
    
        /* Step 2: write the reset type into RSTTRIGCTRLA.SW[26:24] */
        reg  = SMM_RSTTRIGCTRLA_REG;
        reg &= ~(RSTTRIGCTRLA_SW_MSK << RSTTRIGCTRLA_SW_OFF);
        reg |=  ((uint32_t)rstType & RSTTRIGCTRLA_SW_MSK) << RSTTRIGCTRLA_SW_OFF;
        SMM_RSTTRIGCTRLA_REG = reg;
    
        /* Step 3: assert software reset request (SWRSTCON.SWRSTREQ, bit 1) */
        reg  = SMM_SWRSTCON_REG;
        reg |= (SWRSTCON_SWRSTREQ_MSK << SWRSTCON_SWRSTREQ_OFF);
        SMM_SWRSTCON_REG = reg;
    
        /* Step 4: spin until the hardware resets the CPU – never returns */
        for (i = 0U; i < SMM_RESET_DELAY; i++)
        {
            /* intentional empty spin loop */
        }
    }
}
