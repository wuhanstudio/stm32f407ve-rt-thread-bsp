/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 * 2024-04-13     yuanzihao    adaptation for SkyStar STM32F407 version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#include <rtdevice.h>

#include <fal.h>
#include <drv_xpt2046.h>

#define GPIO_LED    GET_PIN(C, 0)

#define TOUCH_DEVICE_NAME "xpt0"
#define TOUCH_DEVICE_SPI "sspi1"

int main(void)
{
    rt_pin_mode(GPIO_LED, PIN_MODE_OUTPUT);

	  fal_init();

    xpt2046_user_init(TOUCH_DEVICE_SPI, GET_PIN(B, 2), GET_PIN(E, 2),
                                        320, 240,
                                        110, 240,
                                        1980, 1870);

    while (1)
    {
        rt_pin_write(GPIO_LED, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
