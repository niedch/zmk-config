const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "lvgl-sdl-demo",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .root_source_file = b.path("src/main.zig"),
        }),
    });

    exe.root_module.addIncludePath(b.path("lvgl/src"));
    exe.root_module.addIncludePath(b.path("lvgl/src/drivers"));
    exe.root_module.addIncludePath(b.path("."));

    const cflags = &[_][]const u8{
        "-DLV_CONF_INCLUDE_SIMPLE",
        "-DSDL_MAIN_HANDLED",
    };

    const lvgl_files = &[_][]const u8{
        // Core
        "lvgl/src/lv_init.c",
        "lvgl/src/core/lv_obj.c",
        "lvgl/src/core/lv_obj_class.c",
        "lvgl/src/core/lv_obj_draw.c",
        "lvgl/src/core/lv_obj_event.c",
        "lvgl/src/core/lv_obj_id_builtin.c",
        "lvgl/src/core/lv_obj_pos.c",
        "lvgl/src/core/lv_obj_property.c",
        "lvgl/src/core/lv_obj_scroll.c",
        "lvgl/src/core/lv_obj_style.c",
        "lvgl/src/core/lv_obj_style_gen.c",
        "lvgl/src/core/lv_obj_tree.c",
        "lvgl/src/core/lv_group.c",
        "lvgl/src/core/lv_refr.c",
        // Display
        "lvgl/src/display/lv_display.c",
        // Draw (software renderer)
        "lvgl/src/draw/lv_draw.c",
        "lvgl/src/draw/lv_draw_arc.c",
        "lvgl/src/draw/lv_draw_buf.c",
        "lvgl/src/draw/lv_draw_image.c",
        "lvgl/src/draw/lv_draw_label.c",
        "lvgl/src/draw/lv_draw_line.c",
        "lvgl/src/draw/lv_draw_mask.c",
        "lvgl/src/draw/lv_draw_rect.c",
        "lvgl/src/draw/lv_draw_triangle.c",
        "lvgl/src/draw/lv_draw_vector.c",
        "lvgl/src/draw/lv_image_decoder.c",
        // Draw SW
        "lvgl/src/draw/sw/lv_draw_sw.c",
        "lvgl/src/draw/sw/lv_draw_sw_arc.c",
        "lvgl/src/draw/sw/lv_draw_sw_border.c",
        "lvgl/src/draw/sw/lv_draw_sw_box_shadow.c",
        "lvgl/src/draw/sw/lv_draw_sw_fill.c",
        "lvgl/src/draw/sw/lv_draw_sw_gradient.c",
        "lvgl/src/draw/sw/lv_draw_sw_img.c",
        "lvgl/src/draw/sw/lv_draw_sw_letter.c",
        "lvgl/src/draw/sw/lv_draw_sw_line.c",
        "lvgl/src/draw/sw/lv_draw_sw_mask.c",
        "lvgl/src/draw/sw/lv_draw_sw_mask_rect.c",
        "lvgl/src/draw/sw/lv_draw_sw_transform.c",
        "lvgl/src/draw/sw/lv_draw_sw_triangle.c",
        "lvgl/src/draw/sw/lv_draw_sw_vector.c",
        // Draw SW blend
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend.c",
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_al88.c",
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.c",
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_i1.c",
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_l8.c",
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.c",
        "lvgl/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.c",
        // Font (add default font)
        "lvgl/src/font/lv_font.c",
        "lvgl/src/font/lv_font_fmt_txt.c",
        "lvgl/src/font/lv_font_montserrat_14.c",
        // Input device
        "lvgl/src/indev/lv_indev.c",
        "lvgl/src/indev/lv_indev_scroll.c",
        // Misc
        "lvgl/src/misc/lv_anim.c",
        "lvgl/src/misc/lv_anim_timeline.c",
        "lvgl/src/misc/lv_area.c",
        "lvgl/src/misc/lv_array.c",
        "lvgl/src/misc/lv_async.c",
        "lvgl/src/misc/lv_bidi.c",
        "lvgl/src/misc/lv_color.c",
        "lvgl/src/misc/lv_color_op.c",
        "lvgl/src/misc/lv_event.c",
        "lvgl/src/misc/lv_fs.c",
        "lvgl/src/misc/lv_ll.c",
        "lvgl/src/misc/lv_log.c",
        "lvgl/src/misc/lv_lru.c",
        "lvgl/src/misc/lv_math.c",
        "lvgl/src/misc/lv_matrix.c",
        "lvgl/src/misc/lv_palette.c",
        "lvgl/src/misc/lv_profiler_builtin.c",
        "lvgl/src/misc/lv_rb.c",
        "lvgl/src/misc/lv_style.c",
        "lvgl/src/misc/lv_style_gen.c",
        "lvgl/src/misc/lv_text.c",
        "lvgl/src/misc/lv_text_ap.c",
        "lvgl/src/misc/lv_timer.c",
        "lvgl/src/misc/lv_utils.c",
        // Cache
        "lvgl/src/misc/cache/lv_cache.c",
        "lvgl/src/misc/cache/lv_cache_entry.c",
        "lvgl/src/misc/cache/lv_cache_lru_rb.c",
        "lvgl/src/misc/cache/lv_image_cache.c",
        "lvgl/src/misc/cache/lv_image_header_cache.c",
        // Stdlib (use built-in)
        "lvgl/src/stdlib/builtin/lv_mem_core_builtin.c",
        "lvgl/src/stdlib/builtin/lv_sprintf_builtin.c",
        "lvgl/src/stdlib/builtin/lv_string_builtin.c",
        "lvgl/src/stdlib/builtin/lv_tlsf.c",
        // Themes
        "lvgl/src/themes/lv_theme.c",
        "lvgl/src/themes/default/lv_theme_default.c",
        "lvgl/src/themes/simple/lv_theme_simple.c",
        "lvgl/src/themes/mono/lv_theme_mono.c",
        // Image decoder (needed for init)
        "lvgl/src/libs/bin_decoder/lv_bin_decoder.c",
        // Tick
        "lvgl/src/tick/lv_tick.c",
        // OSAL
        "lvgl/src/osal/lv_os_none.c",
        // Layouts
        "lvgl/src/layouts/lv_layout.c",
        "lvgl/src/layouts/flex/lv_flex.c",
        "lvgl/src/layouts/grid/lv_grid.c",
        // Widgets
        "lvgl/src/widgets/animimage/lv_animimage.c",
        "lvgl/src/widgets/arc/lv_arc.c",
        "lvgl/src/widgets/bar/lv_bar.c",
        "lvgl/src/widgets/button/lv_button.c",
        "lvgl/src/widgets/buttonmatrix/lv_buttonmatrix.c",
        "lvgl/src/widgets/calendar/lv_calendar.c",
        "lvgl/src/widgets/calendar/lv_calendar_chinese.c",
        "lvgl/src/widgets/calendar/lv_calendar_header_arrow.c",
        "lvgl/src/widgets/calendar/lv_calendar_header_dropdown.c",
        "lvgl/src/widgets/canvas/lv_canvas.c",
        "lvgl/src/widgets/chart/lv_chart.c",
        "lvgl/src/widgets/checkbox/lv_checkbox.c",
        "lvgl/src/widgets/dropdown/lv_dropdown.c",
        "lvgl/src/widgets/image/lv_image.c",
        "lvgl/src/widgets/imagebutton/lv_imagebutton.c",
        "lvgl/src/widgets/keyboard/lv_keyboard.c",
        "lvgl/src/widgets/label/lv_label.c",
        "lvgl/src/widgets/led/lv_led.c",
        "lvgl/src/widgets/line/lv_line.c",
        "lvgl/src/widgets/list/lv_list.c",
        "lvgl/src/widgets/lottie/lv_lottie.c",
        "lvgl/src/widgets/menu/lv_menu.c",
        "lvgl/src/widgets/msgbox/lv_msgbox.c",
        "lvgl/src/widgets/objx_templ/lv_objx_templ.c",
        "lvgl/src/widgets/roller/lv_roller.c",
        "lvgl/src/widgets/scale/lv_scale.c",
        "lvgl/src/widgets/slider/lv_slider.c",
        "lvgl/src/widgets/span/lv_span.c",
        "lvgl/src/widgets/spinbox/lv_spinbox.c",
        "lvgl/src/widgets/spinner/lv_spinner.c",
        "lvgl/src/widgets/switch/lv_switch.c",
        "lvgl/src/widgets/tabview/lv_tabview.c",
        "lvgl/src/widgets/table/lv_table.c",
        "lvgl/src/widgets/textarea/lv_textarea.c",
        "lvgl/src/widgets/tileview/lv_tileview.c",
        "lvgl/src/widgets/win/lv_win.c",
        // Others (needed by theme)
        "lvgl/src/others/imgfont/lv_imgfont.c",
        // SDL Driver
        "lvgl/src/drivers/sdl/lv_sdl_window.c",
        "lvgl/src/drivers/sdl/lv_sdl_mouse.c",
        "lvgl/src/drivers/sdl/lv_sdl_keyboard.c",
        "lvgl/src/drivers/sdl/lv_sdl_mousewheel.c",
    };

    for (lvgl_files) |file| {
        exe.root_module.addCSourceFile(.{ .file = b.path(file), .flags = cflags });
    }

    // Add wrapper functions
    exe.root_module.addCSourceFile(.{ .file = b.path("src/lvgl_wrappers.c"), .flags = cflags });

    exe.root_module.linkSystemLibrary("c", .{});
    exe.root_module.linkSystemLibrary("SDL2", .{});

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the app");
    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
}
