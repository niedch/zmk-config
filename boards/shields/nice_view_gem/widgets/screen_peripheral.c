#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/usb.h>

#include "animation.h"
#include "battery.h"
#include "output.h"
#include "screen_peripheral.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

/**
 * Draw buffers
 **/

static void draw_top(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 0);
    fill_background(canvas);

    // Draw widgets
    draw_output_status(canvas, state);
    draw_battery_status(canvas, state);

    // Rotate for horizontal display
    rotate_canvas(canvas, cbuf);
}

/**
 * Battery status
 **/

static void set_battery_status(struct zmk_widget_screen *widget,
                               struct battery_status_state state) {
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    widget->state.charging = state.usb_present;
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */

    widget->state.battery = state.level;

    draw_top(widget->obj, widget->cbuf, &widget->state);
}

static void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status(widget, state); }
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);

    return (struct battery_status_state){
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state);

ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK) */

/**
 * Peripheral status
 **/

static struct peripheral_status_state get_state(const zmk_event_t *_eh) {
    return (struct peripheral_status_state){.connected = zmk_split_bt_peripheral_is_connected()};
}

static void set_connection_status(struct zmk_widget_screen *widget,
                                  struct peripheral_status_state state) {
    widget->state.connected = state.connected;

    draw_top(widget->obj, widget->cbuf, &widget->state);
}

static void output_status_update_cb(struct peripheral_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_connection_status(widget, state); }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_peripheral_status, struct peripheral_status_state,
                            output_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_peripheral_status, zmk_split_peripheral_status_changed);

/**
 * WPM tracking
 **/

#define WPM_WINDOW_SIZE 100
#define WPM_WINDOW_MS 5000

static uint32_t wpm_timestamps[WPM_WINDOW_SIZE];
static int wpm_head = 0;
static int wpm_count = 0;

static uint32_t current_wpm = 0;

static void update_wpm() {
    uint32_t now = k_uptime_get_32();
    uint32_t cutoff = now - WPM_WINDOW_MS;
    
    // Remove timestamps older than window
    while (wpm_count > 0) {
        int tail = (wpm_head - wpm_count + WPM_WINDOW_SIZE) % WPM_WINDOW_SIZE;
        if (wpm_timestamps[tail] >= cutoff) {
            break;
        }
        wpm_count--;
    }
    
    // Calculate WPM
    if (wpm_count > 0) {
        // WPM = (characters / 5) / (time_seconds / 60)
        // We approximate with window size
        current_wpm = (wpm_count * 60 * 1000) / (WPM_WINDOW_MS * 5);
        if (current_wpm > 255) current_wpm = 255;
    } else {
        current_wpm = 0;
    }
    
    // Update animation speed
    update_animation_speed(current_wpm);
    
    // Update widget states
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        widget->state.wpm = current_wpm;
    }
}

static void record_keypress() {
    uint32_t now = k_uptime_get_32();
    wpm_timestamps[wpm_head] = now;
    wpm_head = (wpm_head + 1) % WPM_WINDOW_SIZE;
    if (wpm_count < WPM_WINDOW_SIZE) {
        wpm_count++;
    }
    update_wpm();
}

// Define the keycode_status_state struct
struct keycode_status_state {
    bool pressed;
};

static void keycode_status_update_cb(struct keycode_status_state state) {
    if (state.pressed) {
        record_keypress();
    }
}

static struct keycode_status_state keycode_get_state(const zmk_event_t *eh) {
    struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    return (struct keycode_status_state){ .pressed = ev->pressed };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_keycode_status, struct keycode_status_state,
                            keycode_status_update_cb, keycode_get_state)
ZMK_SUBSCRIPTION(widget_keycode_status, zmk_keycode_state_changed);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, SCREEN_HEIGHT, SCREEN_WIDTH);

    lv_obj_t *top = lv_canvas_create(widget->obj);
    lv_obj_align(top, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_canvas_set_buffer(top, widget->cbuf, BUFFER_SIZE, BUFFER_SIZE, LV_IMG_CF_TRUE_COLOR);

    draw_animation(widget->obj);

    sys_slist_append(&widgets, &widget->node);
    widget_battery_status_init();
    widget_peripheral_status_init();
    widget_keycode_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
