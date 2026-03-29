#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"


// LV_IMG_DECLARE(bongocatrest0);
// LV_IMG_DECLARE(bongocatcasual1);
// LV_IMG_DECLARE(bongocatcasual2);
// LV_IMG_DECLARE(bongocatfast1);
// LV_IMG_DECLARE(bongocatfast2);
LV_IMG_DECLARE(bongo_resting);
LV_IMG_DECLARE(bongo_casualright);
LV_IMG_DECLARE(bongo_casualleft);
LV_IMG_DECLARE(bongo_furiousup);
LV_IMG_DECLARE(bongo_furiousdown);
LV_IMG_DECLARE(bongo_inhale);
LV_IMG_DECLARE(bongo_exhale);

const lv_img_dsc_t *anim_imgs[] = {
    // &bongocatrest0, &bongocatcasual1, &bongocatcasual2, &bongocatfast1, &bongocatfast2, 
    &bongo_resting, &bongo_casualright, &bongo_casualleft, &bongo_furiousup, &bongo_furiousdown, &bongo_inhale, &bongo_exhale,
};

void draw_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    lv_obj_t *art = lv_animimg_create(canvas);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)anim_imgs, 7);
    lv_animimg_set_duration(art, CONFIG_NICE_VIEW_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
#else
    lv_obj_t *art = lv_img_create(canvas);

    int length = sizeof(anim_imgs) / sizeof(anim_imgs[0]);
    srand(k_uptime_get_32());
    int random_index = rand() % length;

    lv_img_set_src(art, anim_imgs[random_index]);
#endif

    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
}
