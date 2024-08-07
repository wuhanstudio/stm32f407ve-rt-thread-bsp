/**
 * @file lv_demos.h
 *
 */

#ifndef LV_DEMOS_H
#define LV_DEMOS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>

#if LV_USE_DEMO_BENCHMARK
#include "benchmark/lv_demo_benchmark.h"
#endif

#if LV_USE_DEMO_KEYPAD_AND_ENCODER
#include "keypad_encoder/lv_demo_keypad_encoder.h"
#endif

#if LV_USE_DEMO_MUSIC == 1 || defined(LV_USE_DEMO_RTT_MUSIC)
#if LVGL_VERSION_MAJOR < 9
extern void lv_demo_music(void);
#else
#include "music/lv_demo_music.h"
#endif
#endif

#if LV_USE_DEMO_STRESS
#include "stress/lv_demo_stress.h"
#endif

#if LV_USE_DEMO_WIDGETS
#include "widgets/lv_demo_widgets.h"
#endif

#if LV_USE_DEMO_FLEX_LAYOUT
#include "flex_layout/lv_demo_flex_layout.h"
#endif

#if LV_USE_DEMO_TRANSFORM
#include "transform/lv_demo_transform.h"
#endif

#if LV_USE_DEMO_SCROLL
#include "scroll/lv_demo_scroll.h"
#endif

#if LV_USE_DEMO_MULTILANG
#include "multilang/lv_demo_multilang.h"
#endif

#if LV_USE_DEMO_VECTOR_GRAPHIC && LV_USE_VECTOR_GRAPHIC
#include "vector_graphic/lv_demo_vector_graphic.h"
#endif

#if LV_USE_DEMO_RENDER
#include "render/lv_demo_render.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Call lv_demo_xxx.
 * @param   info the information which contains demo name and parameters
 *               needs by lv_demo_xxx.
 * @size    size of information.
 */
bool lv_demos_create(const char *name);

/**
 * Show help for lv_demos.
 */
void lv_demos_show_help(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_H*/
