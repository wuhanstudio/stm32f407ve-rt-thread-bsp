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
#include <lcd.h>
#include <drv_xpt2046.h>

#define GPIO_LED    GET_PIN(C, 0)

extern LCD_DrvTypeDef  *lcd_drv;
extern int touch_calibration(int argc, char* argv[]);

int main(void)
{
    rt_pin_mode(GPIO_LED, PIN_MODE_OUTPUT);

    fal_init();

    /* Touch Init */
    xpt2046_user_init("sspi1", GET_PIN(B, 2), GET_PIN(E, 2),
                                        320, 240,
                                        110, 240,
                                        1980, 1870);
    /* LCD Init */
    lcd_drv->Init();
    lcd_drv->DisplayOn();
	  lcd_drv->FillRect(0, 0, lcd_drv->GetLcdPixelWidth(), lcd_drv->GetLcdPixelHeight(), LCD_COLOR(255, 0, 0));

		touch_calibration(0, RT_NULL);

    while (1)
    {
        rt_pin_write(GPIO_LED, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED, PIN_LOW);
        rt_thread_mdelay(500);
    }
}


/* 
#include <lvgl.h>
#include <rtdevice.h>
#define DBG_TAG "lv_port_indev"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static rt_device_t touch_dev = RT_NULL;
static struct rt_touch_data read_data;
lv_indev_t *indev_touchpad;

static rt_err_t touch_probe()
{
    touch_dev = rt_device_find("xpt0");
    if (touch_dev == RT_NULL)
    {
        LOG_E("can't find device xpt0");
        return -RT_ERROR;
    }
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        LOG_E("open device failed!");
        return -RT_ERROR;
    }
    return RT_EOK;
}
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (rt_device_read(touch_dev, 0, &read_data, 1) == 1)
    {
        data->point.x  = read_data.x_coordinate;
        data->point.y  = read_data.y_coordinate;
        switch (read_data.event)
        {
        case TOUCH_EVENT_UP:
            data->state = LV_INDEV_STATE_REL;
            break;
        case TOUCH_EVENT_DOWN:
            data->state = LV_INDEV_STATE_PR;
            break;
        case TOUCH_EVENT_MOVE:
            data->state = LV_INDEV_STATE_PR;
            break;
        default:
            break;
        }
    }
}

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;
    if (touch_probe() != RT_EOK)
    {
        rt_kprintf("probe xpt0 failed.\n");
        return;
    }

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;

    indev_touchpad = lv_indev_drv_register(&indev_drv);

    LV_IMG_DECLARE(mouse_cursor_icon)
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); 
    lv_img_set_src(cursor_obj, &mouse_cursor_icon); 
    lv_indev_set_cursor(indev_touchpad, cursor_obj);
}
*/
