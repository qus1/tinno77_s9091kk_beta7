/*******************************************************************************
 *
 * Filename:
 * ---------
 *   cfg_wifi_default.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *    give the default MT5921 config data.
 *
 * Author:
 * -------
 *   Renbang (MTK80150) 06/08/2009
 *
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * 09 26 2011 cp.wu
 * [ALPS00070736] MT6573W Wifi wƻݨD
 * 1) update Wi-Fi NVRAM default table
 * 2) add definitions for bird75v1 and eagle15v1
 *
 * 05 31 2011 cp.wu
 * [ALPS00050349] [Need Patch] [Volunteer Patch][MT6620 Wi-Fi][Driver] Add band edge tx power control to Wi-Fi NVRAM
 * adjust default TX power to 20dB(CCK) and 18dB(OFDM/HT20).
 *
 * 05 26 2011 cp.wu
 * [ALPS00050349] [Need Patch] [Volunteer Patch][MT6620 Wi-Fi][Driver] Add band edge tx power control to Wi-Fi NVRAM
 * update Wi-Fi NVRAM definition for band edge tx power control.
 *
 * 11 05 2010 renbang.jiang
 * [ALPS00134025] [Wi-Fi] move Wi-Fi NVRAM definition source file to project folder from common folder
 * .
 *
 * 11 05 2010 renbang.jiang
 * [ALPS00134025] [Wi-Fi] move Wi-Fi NVRAM definition source file to project folder from common folder
 * .
 *
 * 07 10 2010 renbang.jiang
 * [ALPS00121785][Need Patch] [Volunteer Patch] use NVRAM to save Wi-Fi custom data 
 * .
 *
 *    mtk80150
 * [DUMA00131389] [wifi] enable MT5921 NVRAM
 * Enable Daisy Chain in default setting
 *
 * Jul 9 2009 mtk80306
 * [DUMA00122953] optimize nvram and change meta clean boot flag.
 * 
 * 
 *
 *******************************************************************************/



#ifndef _CFG_WIFI_D_H
#define _CFG_WIFI_D_H

#include "../cfgfileinc/CFG_Wifi_File.h"
#include "../inc/wifi_custom.h"
// the default value of Wi-Fi nvram file

#if defined(MT6628)
WIFI_CFG_PARAM_STRUCT stWifiCfgDefault =
{
	0x0104, /* Own Version For MT6628*/
	0x0000, /* Peer Version */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* MAC ADDRESS */
	{ 0x00, 0x00 }, /* COUNTRY CODE */
	{ 0x26, 0x26, { 0x00, 0x00 },
	  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*cTxPwr2G4OFDM*/
      0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*cTxPwr2G4HT20*/
	  0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, /*cTxPwr2G4HT40*/
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*cTxPwr5GOFDM*/
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*cTxPwr5GHT20*/
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /*cTxPwr5GHT40*/ /* TX_PWR_PARAM_T */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* aucEFUSE */
	  0x01, // TX_PWR_PARAM_T is VALID
	  0x00, // 5G band is unsupported
	  0x00, // 2.4GHz band edge power disabled
	  0x26, // cBandEdgeMaxPwrCCK
	  0x20, // cBandEdgeMaxPwrOFDM20
	  0x1C, // cBandEdgeMaxPwrOFDM40
  { 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },	/* aucReserved2 */
  	
    0x0001, /* Own Version */
	  0x0000, /* Peer Version */
		0x0,
		0x0,
		0x0,
		0x0,
  { 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },	/* aucTailReserved */
};
#elif defined(MT5931)
WIFI_CFG_PARAM_STRUCT stWifiCfgDefault =
{
	0x0104, /* Own Version For MT5931*/
	0x0000, /* Peer Version */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* MAC ADDRESS */
	{ 0x00, 0x00 }, /* COUNTRY CODE */
	{ 0x26, 0x26, { 0x00, 0x00 },
	  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*cTxPwr2G4OFDM*/
      0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*cTxPwr2G4HT20*/
	  0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, /*cTxPwr2G4HT40*/
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*cTxPwr5GOFDM*/
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*cTxPwr5GHT20*/
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /*cTxPwr5GHT40*/ /* TX_PWR_PARAM_T */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* aucEFUSE */
	  0x01, // TX_PWR_PARAM_T is VALID
	  0x00, // 5G band is unsupported
	  0x00, // 2.4GHz band edge power disabled
	  0x26, // cBandEdgeMaxPwrCCK
	  0x20, // cBandEdgeMaxPwrOFDM20
	  0x1C, // cBandEdgeMaxPwrOFDM40
  { 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },	/* aucReserved2 */
  	
    0x0001, /* Own Version */
	  0x0000, /* Peer Version */
		0x0,
		0x0,
		0x0,
		0x0,
  { 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },	/* aucTailReserved */
};
#else // for 6620
WIFI_CFG_PARAM_STRUCT stWifiCfgDefault =
{
	0x0103, /* Own Version */
	0x0000, /* Peer Version */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* MAC ADDRESS */
	{ 0x00, 0x00 }, /* COUNTRY CODE */
	{ 0x28, { 0x00, 0x00, 0x00 },
	  0x24, 0x24, 0x24, 0x24,
	  0x24, 0x24, 0x24, 0x24,
	  0x24, 0x24, 0x24, 0x24,
	  0x20, 0x20, 0x20, 0x20,
	  0x20, 0x20, 0x1A, 0x1A,
	  0x1A, 0x1A, 0x1A, 0x1A,
	  0x1A, 0x1A, 0x1A, 0x1A,
	  0x1A, 0x1A, 0x1A, 0x1A,
	  0x1A, 0x1A, 0x1A, 0x1A }, /* TX_PWR_PARAM_T */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* aucEFUSE */
	  0x01, // TX_PWR_PARAM_T is VALID
	  0x00, // 5G band is unsupported
	  0x00, // 2.4GHz band edge power disabled
	  0x00,
	  0x00,
	  0x00,
      0x00,
      0x00,
  { 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00 },

  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* aucReserved2 */
    
    0x0001, /* Own Version */
	  0x0000, /* Peer Version */
		0x0,
		0x0,
		0x0,
		0x0,
  { 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },	/* aucTailReserved */
};
#endif

WIFI_CUSTOM_PARAM_STRUCT stWifiCustomDefault =
{
    0x0, // Reserved
};


#endif
