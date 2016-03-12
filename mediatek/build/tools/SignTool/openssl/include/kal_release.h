/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef SEC_TYPEDEFS_H
#define SEC_TYPEDEFS_H


/* type definition */
#ifdef MT6516
#error "NOT porting yet"
#elif MT6573
#include "mt6573_typedefs.h"
#include "mt6573_partition.h"
#include "mt6573_buffer.h"
#include "mt6573_timer.h"
#include "mt6573.h"
#include "mt65xx.h"
#elif MT6575
#include "mt6575_typedefs.h"
#include "mt6575_partition.h"
#include "mt6575_buffer.h"
#include "mtk_timer.h"
#include "mt6575.h"
#else


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
typedef unsigned char           kal_uint8;
typedef signed char             kal_int8;
typedef char                    kal_char;
typedef unsigned short          kal_wchar;

typedef unsigned short int      kal_uint16;
typedef signed short int        kal_int16;

typedef unsigned int            kal_uint32;
typedef signed int              kal_int32;

typedef unsigned long			u64;
typedef unsigned int            u32;
typedef unsigned char           u8;


typedef unsigned long			__u64;
typedef unsigned int            __u32;
typedef unsigned char           __u8;

typedef unsigned long			__be64;
typedef unsigned int            __be32;
typedef unsigned char           __be8;

//typedef unsigned long long  UINT64X;
//typedef unsigned long long  uint64;
typedef unsigned long   UINT64X;
typedef unsigned long   uint64;


typedef enum 
{
  KAL_FALSE,
  KAL_TRUE
} kal_bool;

#define ASSERT(n) { if (!(n)) { printf("ASSERT ERROR"); } }
//#define _WRITE_DATA_TO_FILE_
//#define _WIN32_DEBUG_


#endif

//#define MSG	printf


#ifndef DOUBLEX
typedef double               DOUBLEX;
//typedef double              DOUBLEX;
#endif 
//#define _REDUCE_CODE_SIZE_

#endif /* SEC_TYPEDEFS_H */


