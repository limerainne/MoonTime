#include "watch.h"

#include "layer_image.h"
#include "layer_text_time.h"

static Window *s_main_window;

static LayerImage layer_image_moon;
static TextLayer *s_time_layer;

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
    layer_text_time_on_update(s_time_layer);
    
    // register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, on_handle_tick);
}

void on_destroy()    {
    // unregister from TickTimeService
    tick_timer_service_unsubscribe();
    
    // destroy Window
    window_destroy(s_main_window);
}

void on_load_window_main(Window *window)    {
    // get property of the Window
    Layer *window_layer = window_get_root_layer(window);
    
    // draw image
    layer_image_create_append_to(&layer_image_moon, window_layer, NULL);
    
    // create the TextLayer with specific bounds
    s_time_layer = layer_text_time_create_append_to(window_layer);
}

void on_unload_window_main(Window *window)    {
    // destroy TextLayer
    text_layer_destroy(s_time_layer);
    
    // destroy image
    layer_image_destroy(&layer_image_moon);
}

void on_handle_tick(struct tm *tick_time, TimeUnits units_changed)    {
    // set text as a current time
    layer_text_time_on_update(s_time_layer);
}
