/*==================================================================================================
* Project : RTD AUTOSAR 4.9
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.9.0
* Autosar Revision : ASR_REL_4_9_REV_0000
* Autosar Conf.Variant :
* SW Version : 7.0.0
* Build Version : S32K3_RTD_7_0_0_QLP03_D2512_ASR_REL_4_9_REV_0000_20251210
*
* Copyright 2020 - 2026 NXP
*
*   NXP Proprietary. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/*
 * TempSense.h
 *
 *  Created on: Feb 20, 2026
 */

#ifndef TEMPSENSE_H_
#define TEMPSENSE_H_

#include "Lpi2c_Ip.h"

#define TEMPSENSE_ADDR		72U
#define LPI2C_INSTANCE		1U
#define TIMEOUT             1000000U



Lpi2c_Ip_StatusType ReadTemperature(uint8 *tempData);
float ConvertTemperature_TMP102(uint8 *tempData);


#endif /* TEMPSENSE_H_ */
