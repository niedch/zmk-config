const std = @import("std");
const c = @cImport({
    @cInclude("lvgl.h");
    @cInclude("drivers/sdl/lv_sdl_window.h");
    @cInclude("drivers/sdl/lv_sdl_mouse.h");
    @cInclude("drivers/sdl/lv_sdl_keyboard.h");
    @cInclude("SDL2/SDL.h");
});

pub fn main() !void {
    // Initialize SDL
    if (c.SDL_Init(c.SDL_INIT_VIDEO) != 0) {
        std.debug.print("SDL init failed: {s}\n", .{c.SDL_GetError()});
        return error.SDLInitFailed;
    }

    // Initialize LVGL
    c.lv_init();

    // Create a default screen BEFORE creating the SDL window
    // This prevents the crash when SDL driver tries to refresh
    _ = c.lv_obj_create(null);

    // Create SDL window display
    const disp = c.lv_sdl_window_create(480, 320);
    if (disp == null) {
        std.debug.print("Failed to create SDL window\n", .{});
        return error.DisplayInitFailed;
    }

    // Initialize mouse input
    _ = c.lv_sdl_mouse_create();

    // Initialize keyboard input
    _ = c.lv_sdl_keyboard_create();

    // Create a simple label on the active screen
    const screen = c.lv_scr_act();
    if (screen == null) {
        std.debug.print("No active screen\n", .{});
        return error.NoScreen;
    }
    const label = c.lv_label_create(screen);
    c.lv_label_set_text(label, "Hello LVGL!");
    c.lv_obj_center(label);

    std.debug.print("LVGL SDL demo running. Press ESC to quit.\n", .{});

    // Main loop
    var quit = false;
    var event: c.SDL_Event = undefined;

    while (!quit) {
        // Handle SDL events
        while (c.SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                c.SDL_QUIT => {
                    quit = true;
                },
                c.SDL_KEYDOWN => {
                    if (event.key.keysym.sym == c.SDLK_ESCAPE) {
                        quit = true;
                    }
                },
                else => {},
            }
        }

        // LVGL task handler
        _ = c.lv_timer_handler();

        // Small delay to prevent busy loop
        c.SDL_Delay(10);
    }

    c.lv_sdl_quit();
    c.SDL_Quit();
}
