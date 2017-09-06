#include "layer_text_time.h"

#include "util.h"

TextLayer* layer_text_time_create_append_to(Layer *window_layer)    {
    GRect bounds = layer_get_bounds(window_layer);
    
    // create the TextLayer with specific bounds
    // NOTE for PBL_IF_ROUND_ELSE() function
    TextLayer *s_time_layer = text_layer_create(
        GRect(PBL_IF_ROUND_ELSE(20, 10), 
              PBL_IF_ROUND_ELSE(90, 84) - 14,    // vertical position is going to set by actual font
              bounds.size.w - 2 * PBL_IF_ROUND_ELSE(20, 10), 
              28));
    
    // improve layout to be more like a watchface (?)
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorLightGray);
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
    
    // reposition regarding height
    // - set dummy text
    text_layer_set_text(s_time_layer, "12:34");
    // - align center vertically
    verticalAlignTextLayer(s_time_layer);
    
    // add layer as a child layer to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    
    return s_time_layer;
}

void layer_text_time_on_update(TextLayer *s_time_layer, struct tm *tick_time)    {   
    if (tick_time == NULL)    { 
        tick_time = get_time_now();
    }
    
    // write 'current' hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), 
            clock_is_24h_style() ? "%H:%M" : "%I:%M",
            tick_time);
    
    // display time text on the TextLayer
    text_layer_set_text(s_time_layer, s_buffer);
}
