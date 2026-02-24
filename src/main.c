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

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/* Including necessary configuration files. */
#include "Mcal.h"
#include "Clock_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Lpi2c_Ip.h"
#include "Lpuart_Uart_Ip.h"
#include "retarget.h"
#include <stdio.h>
#include "math.h"
#include "TempSense.h"
#include "Accel.h"

#define LPI2C1_INSTANCE 		1U
#define SW_DELAY_COUNTER		6000000

int main(void)
{
    /*Declare variables*/
    Lpi2c_Ip_StatusType LpI2cStatus;
	uint8_t rxBuffer_TempSense[2U];
	uint8_t rxBuffer_Accel[6U];
	float temperature_d;
	float accelX, accelY, accelZ;
	int decimal,integer;
    Clock_Ip_StatusType clock_status = CLOCK_IP_ERROR;

    /*Initialize clocks*/
    clock_status =  Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    if (clock_status != CLOCK_IP_SUCCESS)
    {
        while (1);  // Halt
    }

    /*Initialize clocks*/
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals, g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals);
	/*Initialize the LPUART peripheral*/
    Lpuart_Uart_Ip_Init(UART_LPUART_INTERNAL_CHANNEL, &Lpuart_Uart_Ip_xHwConfigPB_6);
    /* Initialize LPI2C peripheral master module */
	Lpi2c_Ip_MasterInit(LPI2C1_INSTANCE, &I2c_Lpi2cMaster_HwChannel1_Channel0);

    /* Initialize accelerometer */
	LpI2cStatus = InitAccelerometer();
    if (LpI2cStatus != LPI2C_IP_SUCCESS_STATUS)
    {
        while (1);  // Halt
    }

    for(;;)
    {
    	/*Read Accel data*/
    	LpI2cStatus = ReadAccelerometer(rxBuffer_Accel);

    	if (LpI2cStatus == LPI2C_IP_SUCCESS_STATUS)
    	{
            /* Convert acceleration data */
            ConvertAcceleration_FXLS8964(rxBuffer_Accel, &accelX, &accelY, &accelZ);

            /* Priority 1: Check for shake - overrides tilt indicator */
            if (DetectShake(accelX, accelY, accelZ))
            {
                HandleShakeAnimation();
            }

            /* Priority 2: Tilt indicator (only if not in shake cooldown) */
            if (GetShakeCooldown() > 0)
            {
                DecrementShakeCooldown();
                TurnOffAllLEDS();
            }
            else
            {
                /* Normal operation - update tilt indicator with different colors */
                UpdateTiltIndicator(accelX, accelY, accelZ);
            }
    	}
    	else
    	{
            while (1);  // Halt

    	}

    	/* Read temperature data */
    	LpI2cStatus = ReadTemperature(rxBuffer_TempSense);
    	if (LpI2cStatus == LPI2C_IP_SUCCESS_STATUS)
    	{
    		/*Take the temperature sensor measurements*/
    		temperature_d = ConvertTemperature_TMP102(rxBuffer_TempSense);
    	}

    	else
    	{
            while (1);  // Halt
    	}

    	/*Converting the the temperature data*/
		integer = (int)temperature_d;
		decimal = (int)((temperature_d-integer)*100);

		/*Print the temperature in the console*/
		printf("\rTemperature is: %d.%02d C\n",integer,decimal);
		printf("\033[1;1H");

		SimpleDelay(SW_DELAY_COUNTER);
    }
    return 0U;
}

/** @} */


#ifdef __cplusplus
}
#endif

/** @} */
