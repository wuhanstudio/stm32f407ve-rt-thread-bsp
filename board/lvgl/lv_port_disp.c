/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-24     Rbb666       The first version
 */
#include <rtthread.h>
#include <lvgl.h>
#include <lcd.h>
#include <ili9341.h>

extern LCD_DrvTypeDef  *lcd_drv;

#if LVGL_VERSION_MAJOR < 9
/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;
/*Descriptor of a display driver*/
static lv_disp_drv_t disp_drv;
#endif

/*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
// static lv_color_t buf_1[ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT];

#if defined ( __GNUC__ )
    static lv_color_t buf_1[ILI9341_LCD_PIXEL_HEIGHT * ILI9341_LCD_PIXEL_WIDTH / 2] __attribute__((section(".LVGLccm")));                          /*A buffer for 10 rows*/
    /*MDK*/
#elif defined ( __CC_ARM )
    __attribute__((at(0x10000000))) lv_color_t buf_1[ILI9341_LCD_PIXEL_WIDTH * ILI9341_LCD_PIXEL_HEIGHT / 2];
#endif

#if LVGL_VERSION_MAJOR < 9
// Flush the content of the internal buffer the specific area on the display
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	  // lcd_drv->DrawRGBImage(area->x1, area->y1, area->x2 - area->x1, area->y2 - area->y1, (uint16_t*) color_p);
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            lcd_drv->WritePixel(x, y, *(uint16_t*)color_p);
            color_p++;
        }
    }
	  lv_disp_flush_ready(disp_drv);
}
#else
static void vsync_wait_cb(lv_display_t *display)
{
    if (!lv_display_flush_is_last(display)) return;
}

static void disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
    if (!lv_display_flush_is_last(display)) return;

    R_GLCDC_BufferChange(&g_display0_ctrl,
                         (uint8_t *) px_map,
                         (display_frame_layer_t) DISPLAY_FRAME_LAYER_1);
}
#endif /* LVGL_VERSION_MAJOR < 9 */

void lv_port_disp_init(void)
{
#if LVGL_VERSION_MAJOR < 9
  
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, lcd_drv->GetLcdPixelWidth() * lcd_drv->GetLcdPixelHeight() / 2);   /*Initialize the display buffer*/

    /*Basic initialization*/
    lv_disp_drv_init(&disp_drv);

    /*Set the resolution of the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;

    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
#else
    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t *disp = lv_display_create(LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_display_set_flush_cb(disp, disp_flush);
    lv_display_set_flush_wait_cb(disp, vsync_wait_cb);
    lv_display_set_buffers(disp, &fb_background[0][0], &fb_background[1][0], sizeof(fb_background[0]), LV_DISPLAY_RENDER_MODE_DIRECT);
#endif
		
		/* LCD Init */
    lcd_drv->Init();
    lcd_drv->DisplayOn();

		// lcd_drv->FillRect(0, 0, lcd_drv->GetLcdPixelWidth(), lcd_drv->GetLcdPixelHeight(), LCD_COLOR(255, 255, 255));

		return;
}
