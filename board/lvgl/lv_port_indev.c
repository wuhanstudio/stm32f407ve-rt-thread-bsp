/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-09     Rbb666       The first version
 */
#include <lvgl.h>
#include <rtthread.h>
#include <rtdevice.h>

#include <ili9341.h>
#include <drv_gpio.h>
#include <drv_xpt2046.h>

#define DBG_TAG "lv_port_indev"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define TOUCH_DEVICE_NAME "xpt0"
#define TOUCH_DEVICE_SPI "sspi1"

static rt_device_t touch_dev;
struct rt_touch_data read_data;
static lv_indev_t *touch_indev;

#if LVGL_VERSION_MAJOR < 9
static void touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data)
#else
static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
#endif
{
    if (touch_dev == RT_NULL)
    {
				//Find the touch device
        touch_dev = rt_device_find(TOUCH_DEVICE_NAME);
        if (touch_dev == RT_NULL)
        {
            LOG_E("Can't find device:%s\n", TOUCH_DEVICE_NAME);
            while (1);
        }
				LOG_I("Found device %s", TOUCH_DEVICE_NAME);
				if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
				{
						LOG_E("Open device failed!");
						while (1);
				}
				LOG_I("Opened device %s", TOUCH_DEVICE_NAME);
    }

		rt_memset(&read_data, 0, sizeof(struct rt_touch_data));

		if (rt_device_read(touch_dev, 0, &read_data, 1) == 1)
		{
			  if (read_data.event == RT_TOUCH_EVENT_NONE)
				{
						return;
				}
				
				// Horizontal
				if (ILI9341_ORIENTATION == 1)
				{
						data->point.x = LV_HOR_RES_MAX - read_data.x_coordinate;
						data->point.y = read_data.y_coordinate;
				}
				else if (ILI9341_ORIENTATION == 3)
				{
						data->point.x = read_data.x_coordinate;
						data->point.y = LV_VER_RES_MAX - read_data.y_coordinate;
				}
				// Vertical
				else if (ILI9341_ORIENTATION == 0)
				{
						data->point.y = LV_VER_RES_MAX - read_data.x_coordinate;
						data->point.x = LV_HOR_RES_MAX - read_data.y_coordinate;
				}
				else if (ILI9341_ORIENTATION == 2)
				{
						data->point.y = read_data.x_coordinate;
						data->point.x = read_data.y_coordinate;
				}

				LOG_I("x: %d, y: %d", data->point.x, data->point.y);

				if (read_data.event == RT_TOUCH_EVENT_DOWN)
						data->state = LV_INDEV_STATE_PR;
				if (read_data.event == RT_TOUCH_EVENT_MOVE)
						data->state = LV_INDEV_STATE_PR;
				if (read_data.event == RT_TOUCH_EVENT_UP)
						data->state = LV_INDEV_STATE_REL;
		}

	  return;
}


void lv_port_indev_init(void)
{
#if LVGL_VERSION_MAJOR < 9
    static lv_indev_drv_t indev_drv;         // Descriptor of a input device driver
    lv_indev_drv_init(&indev_drv);           // Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;  // Touch pad is a pointer-like device
    indev_drv.read_cb = touchpad_read;       // Set your driver function 

    // Register the driver in LVGL and save the created input device object
    touch_indev = lv_indev_drv_register(&indev_drv);
#else
    // Register a touchpad input device
    touch_indev = lv_indev_create();
    lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch_indev, touchpad_read);
#endif
	  /* Touch Init */
    xpt2046_user_init("sspi1", GET_PIN(B, 2), GET_PIN(E, 2),
                                        320, 240,
                                        110, 236,
                                        1980, 1870);
	return;
}
