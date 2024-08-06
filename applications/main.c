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
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

#include <lcd.h>

#define GPIO_LED    GET_PIN(B, 2)

extern LCD_DrvTypeDef  *lcd_drv;

int main(void)
{
    rt_pin_mode(GPIO_LED, PIN_MODE_OUTPUT);

    /* LCD Init */   
    lcd_drv->Init();

    lcd_drv->DisplayOn();
	
		rt_thread_mdelay(1000);

    lcd_drv->FillRect(0, 0, lcd_drv->GetLcdPixelWidth(), lcd_drv->GetLcdPixelHeight(), LCD_COLOR(255, 0, 0));

    while (1)
    {
        rt_pin_write(GPIO_LED, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
