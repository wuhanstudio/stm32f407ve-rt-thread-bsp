#include <rtthread.h>

#include <drv_log.h>
#define LOG_TAG             "drv.xpt2046"

#include <lcd.h>
#include <drv_xpt2046.h>

extern LCD_DrvTypeDef  *lcd_drv;

int touch_calibration(int argc, char* argv[])
{
    const char* touch_name = "xpt0";

    rt_xpt2046_t touch = (rt_xpt2046_t)rt_device_find(touch_name);
    if (touch == RT_NULL)
    {
        LOG_E(LOG_TAG" cannot find touch device named %s\n", touch_name);
        return 1;
    }
    if (rt_device_open((rt_device_t)touch, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        LOG_E(LOG_TAG" cannot open touch device named %s\n", touch_name);
        return 1;
    }

		rt_uint32_t lcd_width = lcd_drv->GetLcdPixelWidth();
		rt_uint32_t lcd_height = lcd_drv->GetLcdPixelHeight();

    lcd_drv->FillRect(0, 0, lcd_drv->GetLcdPixelWidth(), lcd_drv->GetLcdPixelHeight(), LCD_COLOR(255, 255, 255));

    rt_uint32_t cross_size = (lcd_width > lcd_height ? lcd_height : lcd_width) / 10;
    rt_uint32_t x0 = cross_size;
    rt_uint32_t y0 = cross_size;
    rt_uint32_t x1 = lcd_width - cross_size;
    rt_uint32_t y1 = cross_size;
    rt_uint32_t x2 = lcd_width - cross_size;
    rt_uint32_t y2 = lcd_height - cross_size;
    rt_uint32_t x3 = cross_size;
    rt_uint32_t y3 = lcd_height - cross_size;

    // Upper left cross
    lcd_drv->DrawHLine(LCD_COLOR(0, 0, 0), 0, y0, x0+cross_size);
    lcd_drv->DrawVLine(LCD_COLOR(0, 0, 0), x0, 0, y0+cross_size);

    touch->min_raw_x = 0;
    touch->min_raw_y = 0;
    touch->max_raw_x = 4096;
    touch->max_raw_y = 4096;
    touch->parent.info.range_x = 4096;
    touch->parent.info.range_y = 4096;

    rt_uint16_t x_raw[4];
    rt_uint16_t y_raw[4];
    rt_uint8_t raw_idx = 0;
    rt_memset(&x_raw, 0, sizeof(rt_uint32_t)*4);
    rt_memset(&y_raw, 0, sizeof(rt_uint32_t)*4);

    while (1)
    {
        struct rt_touch_data read_data;
        rt_memset(&read_data, 0, sizeof(struct rt_touch_data));
        if (rt_device_read((rt_device_t)touch, 0, &read_data, 1) == 1)
        {
            x_raw[raw_idx] = read_data.x_coordinate;
            y_raw[raw_idx++] = read_data.y_coordinate;
            LOG_I(LOG_TAG" %d point capture", raw_idx-1);

            lcd_drv->FillRect(0, 0, lcd_drv->GetLcdPixelWidth(), lcd_drv->GetLcdPixelHeight(), LCD_COLOR(255, 255, 255));
            rt_thread_mdelay(100);

            if (raw_idx >= 4)
            {
                break;
            }
            switch(raw_idx)
            {
                case 1:
                    // Upper right cross
                    lcd_drv->DrawHLine(LCD_COLOR(0, 0, 0), x1 - cross_size, y1, lcd_width - x1 + cross_size);
                    lcd_drv->DrawVLine(LCD_COLOR(0, 0, 0), x1, 0, y1 + cross_size);
                    break;
                case 2:
                    // lower right cross
                    lcd_drv->DrawHLine(LCD_COLOR(0, 0, 0), x2 - cross_size, y2, lcd_width - x2 + cross_size);
                    lcd_drv->DrawVLine(LCD_COLOR(0, 0, 0), x2, y2 - cross_size, lcd_height - y2 + cross_size);
                    break;
                case 3:
                    // lower left cross
                    lcd_drv->DrawHLine(LCD_COLOR(0, 0, 0), 0, y3, x3 + cross_size);
                    lcd_drv->DrawVLine(LCD_COLOR(0, 0, 0), x3, y3 - cross_size, lcd_height - y3 + cross_size);
                    break;
                default:
                    break;
            }
        }
        rt_thread_mdelay(10);
    }
    rt_uint32_t min_x = (x_raw[2]+x_raw[3])/2;
    rt_uint32_t max_x = (x_raw[0]+x_raw[1])/2;
    rt_uint32_t min_y = (y_raw[1]+y_raw[2])/2;
    rt_uint32_t max_y = (y_raw[0]+y_raw[3])/2;

    rt_uint32_t x_raw_cnt_per_pixel = (max_x-min_x) / (x1-x0);
    rt_uint32_t y_raw_cnt_per_pixel = (max_y-min_y) / (y2-y1);

    min_x -= cross_size * x_raw_cnt_per_pixel;
    max_x += cross_size * x_raw_cnt_per_pixel;
    min_y -= cross_size * y_raw_cnt_per_pixel;
    max_y += cross_size * y_raw_cnt_per_pixel;

    touch->min_raw_x = min_x;
    touch->min_raw_y = min_y;
    touch->max_raw_x = max_x;
    touch->max_raw_y = max_y;
    touch->parent.info.range_x = lcd_width;
    touch->parent.info.range_y = lcd_height;

    LOG_I(LOG_TAG" Calibration result, min_x:%d, min_y:%d, max_x:%d, max_y:%d", min_x, min_y, max_x, max_y);

    rt_device_close((rt_device_t)touch);
		
		return 0;
}
MSH_CMD_EXPORT(touch_calibration, touch calibration)
