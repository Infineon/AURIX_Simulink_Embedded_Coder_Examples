/**
 * \file  SmmReset.h
 * \brief Standalone software-reset trigger for TC49x (no iLLD dependency).
 *
 * Provides a self-contained replacement for IfxSmmRst_triggerReset().
 * Only stdint.h is required.
 */

#ifndef SMM_RESET_H
#define SMM_RESET_H

#include <stdint.h>

/**
 * \brief Software reset trigger type.
 *
 * Mirrors IfxSmmRst_TriggerRstCfgType – values map directly to the
 * RSTTRIGCTRLA.SW bit-field encoding.
 */
typedef enum
{
    SmmReset_Type_noReset           = 0, /**< No reset                  */
    SmmReset_Type_systemReset       = 1, /**< System reset              */
    SmmReset_Type_applicationReset  = 2, /**< Application reset         */
    SmmReset_Type_moduleGroup0Reset = 3, /**< Module group-0 reset      */
    SmmReset_Type_moduleGroup1Reset = 4, /**< Module group-1 reset      */
    SmmReset_Type_moduleGroup2Reset = 5, /**< Module group-2 reset      */
    SmmReset_Type_moduleGroup3Reset = 6  /**< Module group-3 reset      */
} SmmReset_Type;

/**
 * \brief Trigger a software-initiated reset.
 *
 * Unlocks the SMM protection registers, programs the requested reset type
 * into RSTTRIGCTRLA.SW, then asserts SWRSTCON.SWRSTREQ.  A spin-loop
 * provides the hardware time to act.  This function never returns.
 *
 * \param rstType  The type of reset to trigger (see SmmReset_Type).
 */
void SmmReset_triggerReset(int enable);

#endif /* SMM_RESET_H */
