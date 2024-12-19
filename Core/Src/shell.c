/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "log.h"
#include "cmsis_os.h"
#include <stdio.h>


Shell shell;
char shellBuffer[512];

static osMutexId_t shellMutex;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    fwrite(data, 1, len, stdout);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    return fread(data, 1, len, stdin);
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    osMutexAcquire(shellMutex, osWaitForever);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    osMutexRelease(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    osMutexAttr_t mutex_attr = {
        "ShellLock",   // 锁的名称（可选）
        osMutexPrioInherit, // 继承优先级
        NULL,       // 额外的内存区域
        0           // 内存区域的大小
    };

    osThreadAttr_t attr = {
        .name = "shell",
        .stack_size = 256,
        .priority = osPriorityNormal
    };

    shellMutex = osMutexCreate(&mutex_attr);

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
    osThreadNew(shellTask, NULL, &attr);
}
