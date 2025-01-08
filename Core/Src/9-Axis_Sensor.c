#include "FreeRTOS.h"
#include "task.h"

#include <cmsis_os.h>
#include <stdio.h>
#include <icm20948.h>
#include <math.h>

#define GRAVITY 9.80665

#define LOG_TAG "9-Axis"
#include <log.h>

static void NineAxisThreadEntry(void *argument)
{
    axises my_gyro;
    axises my_accel;
    axises my_mag;
    

    while (1)
    {
        // icm20948_gyro_read_dps(&my_gyro);
        icm20948_accel_read_g(&my_accel);
        
        /** 归一化处理 */
        double accel_magnitude = sqrt(my_accel.x * my_accel.x + my_accel.y * my_accel.y + my_accel.z * my_accel.z);

        float pitch_accel = atan2(my_accel.y, my_accel.z) * 180 / M_PI;

        my_accel.x /= accel_magnitude;
        my_accel.y /= accel_magnitude;
        my_accel.z /= accel_magnitude;

        /** 转换为m/s2 */
        my_accel.x *= GRAVITY;
        my_accel.y *= GRAVITY;
        my_accel.z *= GRAVITY;

        logInfo("Accel: x:%.2f, y:%.2f, z:%.2f, a %.2f m/s2 pitch_accel %.2f", my_accel.x, my_accel.y, my_accel.z, accel_magnitude * GRAVITY, pitch_accel);

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
    ak09916_init();

    /** 开启低通滤波器以仅保留重力加速度分量 */
    icm20948_accel_low_pass_filter(6);


    logInfo("ICM-20948 Init Done\n");

    osThreadNew(NineAxisThreadEntry, NULL, &attr);
}
