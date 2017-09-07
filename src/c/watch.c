#include "watch.h"

#include "layer_image.h"
#include "layer_text_time.h"

static Window *s_main_window;

static LayerImage layer_image_moon;

static LayerImage layer_logo_emblem;
static LayerImage layer_logo_sign;

static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

void on_create()    {
    // create main Window element
    s_main_window = window_create();
    
    // set handlers to manage elements inside the window
    window_set_window_handlers(s_main_window, (WindowHandlers){
        .load = on_load_window_main,
        .unload = on_unload_window_main
    });
    
    // show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    
    // make sure the time is displayed from the beginning
    layer_text_time_on_update(s_time_layer, NULL);
    layer_text_date_on_update(s_date_layer, NULL);
    
    // register with TickTimerService
    // NOTE only last called unit/handler pair will be used
    // ref: https://developer.pebble.com/docs/c/Foundation/Event_Service/TickTimerService/#tick_timer_service_subscribe
    // ref: https://stackoverflow.com/questions/27004347/why-does-my-pebble-watchface-not-update-the-time-every-minute
    tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, on_handle_tick);
}

void on_destroy()    {
    // unregister from TickTimeService
    tick_timer_service_unsubscribe();
    
    // destroy Window
    window_destroy(s_main_window);
}

static Layer *s_canvas_layer;

#include "util.h"

static void canvas_update_proc(Layer *layer, GContext *ctx) {
    // Custom drawing happens here!
    
    struct tm *tick_time = get_time_now();
    
    int len_min = 60;
    int len_hrs = 40;

    GPoint start = GPoint(PBL_IF_ROUND_ELSE(90, 72),
                         PBL_IF_ROUND_ELSE(90, 84));
    int angle_min = TRIG_MAX_ANGLE * tick_time->tm_min / 60;    // 6 degree = 360 degree / 60 mins.
    GPoint end_min = GPoint(start.x + sin_lookup(angle_min) * len_min  / TRIG_MAX_RATIO, 
                            start.y + (-1) * cos_lookup(angle_min) * len_min  / TRIG_MAX_RATIO);
    
    // Draw a line
    graphics_context_set_stroke_color(ctx, GColorBulgarianRose);
    graphics_context_set_fill_color(ctx, GColorBulgarianRose);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_draw_line(ctx, start, end_min);
    
    int angle_hrs = TRIG_MAX_ANGLE * tick_time->tm_hour / 12    // 15 degree = 360 degree / 24 hrs.
                + TRIG_MAX_ANGLE * tick_time->tm_min / 720;    // 1 / 4 = 360 degree / 12 hrs. / 60 mins.
    GPoint end_hrs = GPoint(start.x + sin_lookup(angle_hrs) * len_hrs  / TRIG_MAX_RATIO,
                           start.y + (-1) * cos_lookup(angle_hrs) * len_hrs  / TRIG_MAX_RATIO);
    
    graphics_context_set_stroke_color(ctx, GColorBulgarianRose);
    graphics_context_set_fill_color(ctx, GColorBulgarianRose);
    graphics_context_set_stroke_width(ctx, 6);
    graphics_draw_line(ctx, start, end_hrs);
}

void on_load_window_main(Window *window)    {
    // get property of the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_get_root_layer(window));
    
    // set background color (optional)
    window_set_background_color(window, GColorWhite);
    
    // draw image
    // layer_image_create_append_to(&layer_image_moon, window_layer, RESOURCE_ID_BG_POST, bounds);
    layer_image_create_append_to(&layer_logo_emblem, window_layer, RESOURCE_ID_LOGO_EMBLEM,
                                GRect(PBL_IF_ROUND_ELSE(54, 36),
                                     PBL_IF_ROUND_ELSE(25, 20),
                                     72,
                                     46));
    layer_image_create_append_to(&layer_logo_sign, window_layer, RESOURCE_ID_LOGO_SIGN,
                                GRect(PBL_IF_ROUND_ELSE(54, 36),
                                     PBL_IF_ROUND_ELSE(111, 98),
                                     72,
                                     44));
    
    // create the TextLayer with specific bounds
    s_time_layer = layer_text_time_create_append_to(window_layer);
    s_date_layer = layer_text_time_create_append_to(window_layer);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
    
    // draw hands
    s_canvas_layer = layer_create(bounds);
    
    // Assign the custom drawing procedure
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);

    // Add to Window
    layer_add_child(window_get_root_layer(window), s_canvas_layer);
}

void on_unload_window_main(Window *window)    {
    // destroy TextLayer
    text_layer_destroy(s_time_layer);
    
    // destroy image
    // layer_image_destroy(&layer_image_moon);
    
    layer_image_destroy(&layer_logo_emblem);
    layer_image_destroy(&layer_logo_sign);
}

void on_handle_tick(struct tm *tick_time, TimeUnits units_changed)    {
    if ((units_changed & MINUTE_UNIT) != 0)    {
        // set text as a current time
        layer_text_time_on_update(s_time_layer, tick_time);

        // redraw layer
        layer_mark_dirty(s_canvas_layer);
    }
    if ((units_changed & DAY_UNIT) != 0)    {
        layer_text_date_on_update(s_date_layer, tick_time);
    }
}
