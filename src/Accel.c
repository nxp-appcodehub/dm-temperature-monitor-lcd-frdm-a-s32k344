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

#include "Accel.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Dio_Ip.h"
#include <math.h>

static uint32 shake_cooldown = 0;  // Countdown timer for shake animation

void SimpleDelay(uint32 delay)
{
    for (uint32 i = 0; i < delay; i++) {
        __asm("NOP");
    }
}

Lpi2c_Ip_StatusType InitAccelerometer(void) {
    Lpi2c_Ip_StatusType status;
    uint8 writeData[2];
    uint8 whoAmIReg = FXLS8964_WHO_AM_I;
    uint8 whoAmIValue = 0;

    /* Set slave address */
    Lpi2c_Ip_MasterSetSlaveAddr(LPI2C_INSTANCE, ACCEL_ADDR, (boolean) FALSE);

    /* Step 1: Verify communication with WHO_AM_I register */
    status = Lpi2c_Ip_MasterSendDataBlocking(LPI2C_INSTANCE, &whoAmIReg, 1U,
            (boolean) FALSE, TIMEOUT);
    if (status == LPI2C_IP_SUCCESS_STATUS) {
        status = Lpi2c_Ip_MasterReceiveDataBlocking(LPI2C_INSTANCE,
                &whoAmIValue, 1U, (boolean) TRUE, TIMEOUT);

        /* Check if it's FXLS8964 (0x86) or FXLS8974 (0x87) */
        if (whoAmIValue != 0x86U && whoAmIValue != 0x87U) {
            return LPI2C_IP_ERROR_STATUS;  // Wrong device
        }
    } else {
        return status;  // Communication failed
    }

    SimpleDelay(10000U);

    /* Step 2: Enter Standby mode (required for configuration) */
    writeData[0] = FXLS8964_SENS_CONFIG1;
    writeData[1] = 0x00U;  // ACTIVE=0, FSR=00b (±2g)
    status = Lpi2c_Ip_MasterSendDataBlocking(LPI2C_INSTANCE, writeData, 2U,
            (boolean) TRUE, TIMEOUT);
    if (status != LPI2C_IP_SUCCESS_STATUS)
        return status;

    SimpleDelay(10000U);

    /* Step 3: Configure data format - CRITICAL! */
    writeData[0] = FXLS8964_SENS_CONFIG2;
    writeData[1] = 0x00U;  // LE_BE=0 (little-endian), F_READ=0 (12-bit), LPM
    status = Lpi2c_Ip_MasterSendDataBlocking(LPI2C_INSTANCE, writeData, 2U,
            (boolean) TRUE, TIMEOUT);
    if (status != LPI2C_IP_SUCCESS_STATUS)
        return status;

    SimpleDelay(10000U);

    /* Step 4: Set ODR to 100 Hz */
    writeData[0] = FXLS8964_SENS_CONFIG3;
    writeData[1] = 0x55U;  // WAKE_ODR=100Hz, SLEEP_ODR=100Hz
    status = Lpi2c_Ip_MasterSendDataBlocking(LPI2C_INSTANCE, writeData, 2U,
            (boolean) TRUE, TIMEOUT);
    if (status != LPI2C_IP_SUCCESS_STATUS)
        return status;

    SimpleDelay(10000U);

    /* Step 5: Enter Active mode */
    writeData[0] = FXLS8964_SENS_CONFIG1;  // SENS_CONFIG1
    writeData[1] = 0x01U;  // ACTIVE=1, FSR=00b (±2g)
    status = Lpi2c_Ip_MasterSendDataBlocking(LPI2C_INSTANCE, writeData, 2U,
            (boolean) TRUE, TIMEOUT);

    SimpleDelay(50000U);  // Wait for sensor to stabilize

    return status;
}

Lpi2c_Ip_StatusType ReadAccelerometer(uint8 *accelData) {
    Lpi2c_Ip_StatusType status;
    uint8 regAddr = FXLS8964_OUT_X_LSB; /* Start at X-axis LSB register (0x04) */

    /* Step 1: Set slave address */
    Lpi2c_Ip_MasterSetSlaveAddr(LPI2C_INSTANCE, ACCEL_ADDR, (boolean) FALSE);

    /* Step 2: Send register address (write operation) */
    status = Lpi2c_Ip_MasterSendDataBlocking(
    LPI2C_INSTANCE, &regAddr, 1U, (boolean) FALSE, TIMEOUT);

    if (status == LPI2C_IP_SUCCESS_STATUS) {
        /* Step 3: Read 6 bytes (X, Y, Z acceleration data) */
        status = Lpi2c_Ip_MasterReceiveDataBlocking(
        LPI2C_INSTANCE, accelData, 6U, (boolean) TRUE, TIMEOUT);
    }
    return status;
}

void ConvertAcceleration_FXLS8964(uint8 *accelData, float *x, float *y, float *z) {
    // The sensor provides sign-extended 16-bit 2's complement values
    int16_t rawX = (int16_t) ((accelData[1] << 8) | accelData[0]);
    int16_t rawY = (int16_t) ((accelData[3] << 8) | accelData[2]);
    int16_t rawZ = (int16_t) ((accelData[5] << 8) | accelData[4]);

    if (rawX & 0x0800)
        rawX |= 0xF000;
    if (rawY & 0x0800)
        rawY |= 0xF000;
    if (rawZ & 0x0800)
        rawZ |= 0xF000;

    const float G_PER_LSB = 1 / 1024.0f;
    const float G_TO_MS2 = 9.80665f;

    *x = rawX * G_PER_LSB * G_TO_MS2;
    *y = rawY * G_PER_LSB * G_TO_MS2;
    *z = rawZ * G_PER_LSB * G_TO_MS2;
}


void TurnOffAllLEDS(void)
{
    Siul2_Dio_Ip_WritePin(RED_LED_PORT, RED_LED_PIN, 0U);
    Siul2_Dio_Ip_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, 0U);
    Siul2_Dio_Ip_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, 0U);

}

void TurnOnAllLEDS(void) {
    Siul2_Dio_Ip_WritePin(RED_LED_PORT, RED_LED_PIN, 1U);
    Siul2_Dio_Ip_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, 1U);
    Siul2_Dio_Ip_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, 1U);
}

void SetLEDColor(uint8 red, uint8 green, uint8 blue) {
    Siul2_Dio_Ip_WritePin(RED_LED_PORT, RED_LED_PIN, red);
    Siul2_Dio_Ip_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, green);
    Siul2_Dio_Ip_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, blue);
}

uint8 DetectShake(float x, float y, float z) {
    static float prev_x = 0, prev_y = 0, prev_z = 0;
    static uint8 first_run = 1;

    /* Skip first run to initialize previous values */
    if (first_run) {
        prev_x = x;
        prev_y = y;
        prev_z = z;
        first_run = 0;
        return 0;
    }

    /* Calculate change in acceleration */
    float delta_x = fabsf(x - prev_x);
    float delta_y = fabsf(y - prev_y);
    float delta_z = fabsf(z - prev_z);

    float total_delta = delta_x + delta_y + delta_z;

    /* Store current values for next iteration */
    prev_x = x;
    prev_y = y;
    prev_z = z;

    /* Return TRUE if shake detected */
    if (total_delta > 15.0f) {
        return 1;  // Shake detected!
    }

    return 0;  // No shake
}

void HandleShakeAnimation(void) {
    /* Flash pattern: 3 quick flashes */
    for (uint8 i = 0; i < 3; i++) {
        TurnOnAllLEDS();
        SimpleDelay(100000U);
        TurnOffAllLEDS();
        SimpleDelay(100000U);
    }

    /* Set cooldown to prevent re-triggering immediately */
    shake_cooldown = 10;  // Skip next 10 tilt updates
}

void UpdateTiltIndicator(float x, float y, float z) {
    float accelX_abs = fabsf(x);
    float accelY_abs = fabsf(y);
    float accelZ_abs = fabsf(z);

    /* Turn off all LEDs first */
    TurnOffAllLEDS();

    /* Light LED based on dominant axis */
    if (accelX_abs > AXIS_THRESHOLD) {
        if (accelY_abs < AXIS_THRESHOLD && accelZ_abs < AXIS_THRESHOLD) {
            /* X-axis dominant */
            if (x > 0) {
                /* Positive X: Pure RED */
                SetLEDColor(1, 0, 0);
            } else {
                /* Negative X: ORANGE (Red + Green) */
                SetLEDColor(1, 1, 0);
            }
        }
    }
    else if (accelY_abs > AXIS_THRESHOLD) {
        if (accelX_abs < AXIS_THRESHOLD && accelZ_abs < AXIS_THRESHOLD) {
            /* Y-axis dominant */
            if (y > 0) {
                /* Positive Y: Pure GREEN */
                SetLEDColor(0, 1, 0);
            } else {
                /* Negative Y: CYAN (Green + Blue) */
                SetLEDColor(0, 1, 1);
            }
        }
    }
    else if (accelZ_abs > AXIS_THRESHOLD) {
        if (accelY_abs < AXIS_THRESHOLD && accelX_abs < AXIS_THRESHOLD) {
            /* Z-axis dominant */
            if (z > 0) {
                /* Positive Z: Pure BLUE */
                SetLEDColor(0, 0, 1);
            } else {
                /* Negative Z: MAGENTA (Red + Blue) */
                SetLEDColor(1, 0, 1);
            }
        }
    }
}

uint32 GetShakeCooldown(void) {
    return shake_cooldown;
}

void DecrementShakeCooldown(void) {
    if (shake_cooldown > 0) {
        shake_cooldown--;
    }
}

