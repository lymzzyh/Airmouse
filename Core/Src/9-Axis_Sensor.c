#include "FreeRTOS.h"
#include "task.h"

#include <cmsis_os.h>
#include <stdio.h>
#include <icm20948.h>

#define LOG_TAG "9-Axis"
#include <log.h>

static void NineAxisThreadEntry(void *argument)
{
    axises my_gyro;
    axises my_accel;
    axises my_mag;
    

    while (1)
    {
        icm20948_gyro_read_dps(&my_gyro);
        icm20948_accel_read_g(&my_accel);
        ak09916_mag_read_uT(&my_mag);

        logInfo("Accel: %.2fdps, %.2fdps, %.2fdps", my_accel.x, my_accel.y, my_accel.z);
        logInfo("Gyro: %.2fg, %.2fg, %.2fg", my_gyro.x, my_gyro.y, my_gyro.z);
        logInfo("Mag: %.2fuT, %.2fuT, %.2fuT", my_mag.x, my_mag.y, my_mag.z);

        osDelay(1000);
    }
}



void NineAxisSensorInit(void)
{
    osThreadAttr_t attr = {
        .name = "9-Axis Sensor",
        .stack_size = 512,
        .priority = osPriorityNormal1
    };

    icm20948_init();    

    logInfo("ICM-20948 Init Done\n");

    osThreadNew(NineAxisThreadEntry, NULL, &attr);
}
