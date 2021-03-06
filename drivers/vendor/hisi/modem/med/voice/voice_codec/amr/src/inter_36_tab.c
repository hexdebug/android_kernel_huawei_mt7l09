/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : inter_36.tab
*      Purpose          : Tables for interpolating the normalized correlation
*                         with 1/3 or 1/6 resolution.
*      $Id $
*
********************************************************************************
*/
#include "codec_op_etsi.h"
#include "cnst.h"

/* originally defined at inter_36.c */
#define UP_SAMP_MAX                     6
#define FIR_SIZE                        (UP_SAMP_MAX*L_INTER_SRCH+1)

/* 1/6 resolution interpolation filter  (-3 dB at 3600 Hz) */
/* Note: The IS641 (7.4) 1/3 resolution filter is simply a subsampled
         version of the 1/6 resolution filter, i.e. it uses
         every second coefficient:

         inter_3[k] = inter_6[2*k], 0 <= k <= 3*L_INTER_SRCH
 */

static const Word16 inter_6[FIR_SIZE] =
{
    29519,
    28316, 24906, 19838, 13896, 7945, 2755,
    -1127, -3459, -4304, -3969, -2899, -1561,
    -336, 534, 970, 1023, 823, 516,
    220, 0, -131, -194, -215, 0
};
