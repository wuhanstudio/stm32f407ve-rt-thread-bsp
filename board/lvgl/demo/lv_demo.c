/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-17     Meco Man      First version
 * 2022-05-10     Meco Man      improve rt-thread initialization process
 */

#include <lvgl.h>
#include "rtconfig.h"
#include "lv_demos.h"

#define DBG_TAG "lv_demo"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/*********************
 *      DEFINES
 *********************/
#define LV_DEMOS_COUNT (sizeof(demos_entry_info) / sizeof(demo_entry_info_t) - 1)

/**********************
 *      TYPEDEFS
 **********************/

typedef void (*demo_method_cb)(void);

typedef struct
{
    const char *name;
    demo_method_cb entry_cb;
} demo_entry_info_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static const demo_entry_info_t demos_entry_info[] =
{
#if LV_USE_DEMO_WIDGETS
    { "widgets", .entry_cb = lv_demo_widgets },
#endif

#if LV_USE_DEMO_MUSIC == 1 || defined(LV_USE_DEMO_RTT_MUSIC)
    { "music", .entry_cb = lv_demo_music },
#endif

#if LV_USE_DEMO_STRESS
    { "stress", .entry_cb = lv_demo_stress },
#endif

#if LV_USE_DEMO_BENCHMARK
    { "benchmark", .entry_cb = lv_demo_benchmark },
#endif

    { "", .entry_cb = NULL }
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool lv_demos_create(const char *name)
{
    const int demos_count = LV_DEMOS_COUNT;

    if (demos_count <= 0)
    {
        LV_LOG_ERROR("Please enable some lv_demos firstly!");
        return false;
    }

    const demo_entry_info_t *entry_info = NULL;
    for (int i = 0; i < demos_count; i++)
    {
        if (rt_strcmp(name, demos_entry_info[i].name) == 0)
        {
            entry_info = &demos_entry_info[i];
        }
    }

    if (entry_info->entry_cb)
    {
        entry_info->entry_cb();
        return true;
    }

    return false;
}

void lv_demos_show_help(void)
{
    int i;
    const int demos_count = LV_DEMOS_COUNT;

    if (demos_count == 0)
    {
        LV_LOG("lv_demos: no demo available!\n");
        return;
    }

    LV_LOG("\nUsage: lv_demos demo [parameters]\n");
    LV_LOG("\ndemo list:\n");

    for (i = 0; i < demos_count; i++)
    {
        LV_LOG("     %s \n", demos_entry_info[i].name);
    }
}

#if LVGL_VERSION_MAJOR < 9
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_PRESSED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
}
#endif // LVGL_VERSION_MAJOR < 9

void lv_user_gui_init(void)
{
    LOG_I("Running LVGL Demo");

    /* display demo; you may replace with your LVGL application at here */
#if LV_USE_DEMO_MUSIC == 1 || defined(LV_USE_DEMO_RTT_MUSIC)
    lv_demos_create("music");
#elif LV_USE_DEMO_BENCHMARK
    lv_demos_create("benchmark");
#elif LV_USE_DEMO_WIDGETS
    lv_demos_create("widgets");
#elif LV_USE_DEMO_STRESS
    lv_demos_create("stress");
#else
	#if LVGL_VERSION_MAJOR < 9
    lv_example_get_started_1();
	#endif
	lv_demos_show_help();
#endif
}
