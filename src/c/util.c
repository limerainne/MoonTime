#include "util.h"

struct tm* get_time_now()    {
    // get a 'tm' structure
    time_t time_standard = time(NULL);
    struct tm *tick_time = localtime(&time_standard);    
    
    return tick_time;
}

// ref: https://forums.pebble.com/t/vertical-text-alignment/9788/10
void verticalAlignTextLayer(TextLayer *layer) {
    GRect frame = layer_get_frame(text_layer_get_layer(layer));
    GSize content = text_layer_get_content_size(layer);
    layer_set_frame(text_layer_get_layer(layer),
           GRect(frame.origin.x, frame.origin.y + (frame.size.h - content.h - 5) / 2,     // XXX why '-5' ??
           frame.size.w, content.h));
}