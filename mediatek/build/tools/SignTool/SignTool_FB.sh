#!/bin/sh
# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


##############################################################
# Program:
#	SIGN TOOL
#

SignTool=${0%/*}/SignTool

#{ARIMA
echo "[FB_SIG make2 -ARIMA sign_tool.sh -1]  $1"
echo "[FB_SIG make2 -ARIMA sign_tool.sh -2]  $2"
echo "[FB_SIG make2-ARIMA sign_tool.sh -3]  $3"
echo "[FB_SIG make2 -ARIMA sign_tool.sh -4]  $4"
echo "[FB_SIG make2 -ARIMA sign_tool.sh -5]  $5"
echo "[FB_SIG make2 -ARIMA sign_tool.sh -6]  $6"
echo "[FB_SIG make2 -ARIMA sign_tool.sh -7]  $7"
echo "[FB_SIG make 2-ARIMA sign_tool.sh -8]  $8"
echo "[FB_SIG make 2-ARIMA sign_tool $SignTool $1 $2 $4 $3 FB_SIG"

#}ARIMA

##############################################################
# Check if need to generate fastboot signature

#if [ "$6" == "FB_SIG" ] ; then
    ./${SignTool} $1 $2 $4 $3 FB_SIG
    if [ $? -eq 0 ] ; then
        echo "SIGN PASS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!(FB SIG)"
    else
        echo "SIGN FAIL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!(FB SIG)"
        exit 1;
    fi
    cp -f FB_SIG $4.csd
    cat $4 >> $4.csd
    
    rm -f FB_SIG
    rm -f $4.sig
    rm -f $4
    mv -f $4.csd $4
#fi

exit 0;