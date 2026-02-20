#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"

void draw_animation(lv_obj_t *canvas) {
    lv_obj_t *border = lv_obj_create(canvas);
    lv_obj_set_size(border, 156, 64);
    lv_obj_set_style_bg_opa(border, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(border, lv_color_white(), 0);
    lv_obj_set_style_border_width(border, 2, 0);
    lv_obj_set_style_border_side(border, LV_BORDER_SIDE_FULL, 0);
    lv_obj_align(border, LV_ALIGN_TOP_LEFT, 2, 2);
}