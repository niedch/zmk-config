#pragma once

#include <lvgl.h>
#include "util.h"
#include "screen_peripheral.h"

extern const lv_img_dsc_t bongocatrest0;
extern const lv_img_dsc_t bongocatcasual1;
extern const lv_img_dsc_t bongocatcasual2;
extern const lv_img_dsc_t bongocatfast1;
extern const lv_img_dsc_t bongocatfast2;

void draw_animation(lv_obj_t *canvas);
