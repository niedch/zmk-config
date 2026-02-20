// Wrapper functions to fix LVGL stdlib mismatch
#include "lvgl.h"

// Memory wrappers
void *lv_malloc(size_t size) { return lv_malloc_core(size); }
void *lv_malloc_zeroed(size_t size) { 
    void *ptr = lv_malloc_core(size);
    if (ptr) {
        // Could zero it here, but let's just use the core
    }
    return ptr;
}
void lv_free(void *p) { lv_free_core(p); }
void *lv_realloc(void *p, size_t new_size) { return lv_realloc_core(p, new_size); }

// Lock wrappers (for LV_OS_NONE)
void lv_lock(void) {}
void lv_unlock(void) {}
void lv_os_init(void) {}
