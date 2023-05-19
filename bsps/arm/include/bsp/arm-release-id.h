/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsARMShared
 *
 * @brief Create #defines for release IDs
 */

/*
 * Copyright (c) 2014 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ARM_RELEASE_ID_H_
#define ARM_RELEASE_ID_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* #define the IDs which identify a release. This must be done
 * prior to #including bsp.h */
#define ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( release_no, patch_level ) \
  ( ( ( ( release_no ) & 0xFFFF ) << 16 ) + ( ( patch_level ) & 0xFFFF ) )

typedef enum {
  ARM_RELEASE_ID_R0_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 0 ),
  ARM_RELEASE_ID_R0_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 1 ),
  ARM_RELEASE_ID_R0_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 2 ),
  ARM_RELEASE_ID_R0_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 3 ),
  ARM_RELEASE_ID_R0_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 4 ),
  ARM_RELEASE_ID_R0_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 5 ),
  ARM_RELEASE_ID_R0_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 6 ),
  ARM_RELEASE_ID_R0_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 7 ),
  ARM_RELEASE_ID_R0_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 8 ),
  ARM_RELEASE_ID_R0_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 9 ),
  ARM_RELEASE_ID_R0_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 0, 10 ),
  ARM_RELEASE_ID_R1_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 0 ),
  ARM_RELEASE_ID_R1_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 1 ),
  ARM_RELEASE_ID_R1_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 2 ),
  ARM_RELEASE_ID_R1_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 3 ),
  ARM_RELEASE_ID_R1_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 4 ),
  ARM_RELEASE_ID_R1_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 5 ),
  ARM_RELEASE_ID_R1_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 6 ),
  ARM_RELEASE_ID_R1_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 7 ),
  ARM_RELEASE_ID_R1_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 8 ),
  ARM_RELEASE_ID_R1_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 9 ),
  ARM_RELEASE_ID_R1_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 1, 10 ),
  ARM_RELEASE_ID_R2_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 0 ),
  ARM_RELEASE_ID_R2_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 1 ),
  ARM_RELEASE_ID_R2_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 2 ),
  ARM_RELEASE_ID_R2_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 3 ),
  ARM_RELEASE_ID_R2_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 4 ),
  ARM_RELEASE_ID_R2_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 5 ),
  ARM_RELEASE_ID_R2_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 6 ),
  ARM_RELEASE_ID_R2_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 7 ),
  ARM_RELEASE_ID_R2_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 8 ),
  ARM_RELEASE_ID_R2_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 9 ),
  ARM_RELEASE_ID_R2_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 2, 10 ),
  ARM_RELEASE_ID_R3_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 0 ),
  ARM_RELEASE_ID_R3_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 1 ),
  ARM_RELEASE_ID_R3_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 2 ),
  ARM_RELEASE_ID_R3_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 3 ),
  ARM_RELEASE_ID_R3_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 4 ),
  ARM_RELEASE_ID_R3_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 5 ),
  ARM_RELEASE_ID_R3_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 6 ),
  ARM_RELEASE_ID_R3_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 7 ),
  ARM_RELEASE_ID_R3_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 8 ),
  ARM_RELEASE_ID_R3_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 9 ),
  ARM_RELEASE_ID_R3_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 3, 10 ),
  ARM_RELEASE_ID_R4_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 0 ),
  ARM_RELEASE_ID_R4_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 1 ),
  ARM_RELEASE_ID_R4_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 2 ),
  ARM_RELEASE_ID_R4_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 3 ),
  ARM_RELEASE_ID_R4_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 4 ),
  ARM_RELEASE_ID_R4_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 5 ),
  ARM_RELEASE_ID_R4_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 6 ),
  ARM_RELEASE_ID_R4_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 7 ),
  ARM_RELEASE_ID_R4_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 8 ),
  ARM_RELEASE_ID_R4_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 9 ),
  ARM_RELEASE_ID_R4_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 4, 10 ),
  ARM_RELEASE_ID_R5_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 0 ),
  ARM_RELEASE_ID_R5_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 1 ),
  ARM_RELEASE_ID_R5_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 2 ),
  ARM_RELEASE_ID_R5_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 3 ),
  ARM_RELEASE_ID_R5_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 4 ),
  ARM_RELEASE_ID_R5_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 5 ),
  ARM_RELEASE_ID_R5_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 6 ),
  ARM_RELEASE_ID_R5_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 7 ),
  ARM_RELEASE_ID_R5_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 8 ),
  ARM_RELEASE_ID_R5_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 9 ),
  ARM_RELEASE_ID_R5_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 5, 10 ),
  ARM_RELEASE_ID_R6_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 0 ),
  ARM_RELEASE_ID_R6_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 1 ),
  ARM_RELEASE_ID_R6_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 2 ),
  ARM_RELEASE_ID_R6_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 3 ),
  ARM_RELEASE_ID_R6_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 4 ),
  ARM_RELEASE_ID_R6_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 5 ),
  ARM_RELEASE_ID_R6_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 6 ),
  ARM_RELEASE_ID_R6_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 7 ),
  ARM_RELEASE_ID_R6_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 8 ),
  ARM_RELEASE_ID_R6_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 9 ),
  ARM_RELEASE_ID_R6_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 6, 10 ),
  ARM_RELEASE_ID_R7_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 0 ),
  ARM_RELEASE_ID_R7_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 1 ),
  ARM_RELEASE_ID_R7_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 2 ),
  ARM_RELEASE_ID_R7_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 3 ),
  ARM_RELEASE_ID_R7_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 4 ),
  ARM_RELEASE_ID_R7_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 5 ),
  ARM_RELEASE_ID_R7_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 6 ),
  ARM_RELEASE_ID_R7_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 7 ),
  ARM_RELEASE_ID_R7_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 8 ),
  ARM_RELEASE_ID_R7_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 9 ),
  ARM_RELEASE_ID_R7_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 7, 10 ),
  ARM_RELEASE_ID_R8_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 0 ),
  ARM_RELEASE_ID_R8_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 1 ),
  ARM_RELEASE_ID_R8_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 2 ),
  ARM_RELEASE_ID_R8_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 3 ),
  ARM_RELEASE_ID_R8_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 4 ),
  ARM_RELEASE_ID_R8_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 5 ),
  ARM_RELEASE_ID_R8_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 6 ),
  ARM_RELEASE_ID_R8_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 7 ),
  ARM_RELEASE_ID_R8_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 8 ),
  ARM_RELEASE_ID_R8_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 9 ),
  ARM_RELEASE_ID_R8_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 8, 10 ),
  ARM_RELEASE_ID_R9_P0  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 0 ),
  ARM_RELEASE_ID_R9_P1  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 1 ),
  ARM_RELEASE_ID_R9_P2  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 2 ),
  ARM_RELEASE_ID_R9_P3  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 3 ),
  ARM_RELEASE_ID_R9_P4  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 4 ),
  ARM_RELEASE_ID_R9_P5  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 5 ),
  ARM_RELEASE_ID_R9_P6  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 6 ),
  ARM_RELEASE_ID_R9_P7  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 7 ),
  ARM_RELEASE_ID_R9_P8  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 8 ),
  ARM_RELEASE_ID_R9_P9  = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 9 ),
  ARM_RELEASE_ID_R9_P10 = ARM_RELEASE_ID_FROM_NUMBER_AND_PATCH_LEVEL( 9, 10 )
} arm_release_id;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ARM_RELEASE_ID_H_ */