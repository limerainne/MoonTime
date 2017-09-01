#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static BitmapLayer *meri_image_layer;
static GBitmap *meri_image;

static void update_time()    {
    // get a 'tm' structure
    time_t time_standard = time(NULL);
    struct tm *tick_time = localtime(&time_standard);
    
    // write 'current' hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), 
            clock_is_24h_style() ? "%H:%M" : "%I:%M",
            tick_time);
    
    // display time text on the TextLayer
    text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)    {
    // set text as a current time
    update_time();
}

static void main_window_load(Window *window)    {
    // get property of the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    // draw image
      meri_image_layer = bitmap_layer_create(bounds);
      meri_image = gbitmap_create_with_resource(RESOURCE_ID_BG_POST);
    	
      bitmap_layer_set_compositing_mode(meri_image_layer, GCompOpAssign);
      bitmap_layer_set_bitmap(meri_image_layer, meri_image);
      bitmap_layer_set_alignment(meri_image_layer, GAlignCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(meri_image_layer));
    
    // create the TextLayer with specific bounds
    // NOTE for PBL_IF_ROUND_ELSE() function
    s_time_layer = text_layer_create(
        GRect(PBL_IF_ROUND_ELSE(40, 10), 
              PBL_IF_ROUND_ELSE(4, -4), 
              bounds.size.w - 2 * PBL_IF_ROUND_ELSE(40, 10), 
              PBL_IF_ROUND_ELSE(28, 28)));
    
    // improve layout to be more like a watchface (?)
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
    
    // add layer as a child layer to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window)    {
    // destroy TextLayer
    text_layer_destroy(s_time_layer);
    
    // destroy image
      gbitmap_destroy(meri_image);
      bitmap_layer_destroy(meri_image_layer);
}

static void init()    {
    // create main Window element
    s_main_window = window_create();
    
    // set handlers to manage elements inside the window
    window_set_window_handlers(s_main_window, (WindowHandlers){
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    // show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    
    // make sure the time is displayed from the beginning
    update_time();
    
    // register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit()    {
    // unregister from TickTimeService
    // NOTE no need to do this?
    tick_timer_service_unsubscribe();
    
    // destroy Window
    window_destroy(s_main_window);
}

int main(void)    {
    init();
    
    app_event_loop();
    
    deinit();
    
    return 0;
}
