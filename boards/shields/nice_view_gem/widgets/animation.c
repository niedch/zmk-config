#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"

LV_IMG_DECLARE(mario1standing);
LV_IMG_DECLARE(mario2run1);
LV_IMG_DECLARE(mario2run2);
LV_IMG_DECLARE(mario2run3);
LV_IMG_DECLARE(mario3jump1);
LV_IMG_DECLARE(mario3jump2);
LV_IMG_DECLARE(mario3jump3);
LV_IMG_DECLARE(mario3jump4);
LV_IMG_DECLARE(mario3jump5);
LV_IMG_DECLARE(mario3jump6);
LV_IMG_DECLARE(mario3jump7);
LV_IMG_DECLARE(mario3jump8);
LV_IMG_DECLARE(mario3jump9);
LV_IMG_DECLARE(mario3jump10);
LV_IMG_DECLARE(mario3jump11);
LV_IMG_DECLARE(mario3jump12);
LV_IMG_DECLARE(mario3jump13);

const lv_img_dsc_t *anim_imgs[] = {
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario1standing,&mario1standing,&mario1standing,&mario1standing,&mario1standing,
    &mario2run1,&mario2run2,&mario2run3,
    &mario2run1,&mario2run2,&mario2run3,
    &mario2run1,&mario2run2,&mario2run3,
    &mario2run1,&mario2run2,&mario2run3,
    &mario2run1,&mario2run2,&mario2run3,
    &mario3jump1,
    &mario3jump2,
    &mario3jump3,
    &mario3jump4,
    &mario3jump5,
    &mario3jump6,
    &mario3jump7,
    &mario3jump8,
    &mario3jump9,
    &mario3jump10,
    &mario3jump11,
    &mario3jump12,
    &mario3jump13,
};

void draw_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    lv_obj_t *art = lv_animimg_create(canvas);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)anim_imgs, 78);
    lv_animimg_set_duration(art, 7800);
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
