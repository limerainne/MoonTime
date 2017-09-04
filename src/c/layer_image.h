#pragma once

#include <pebble.h>

typedef struct layer_image_t    {
    BitmapLayer *s_layer;
    GBitmap *s_bitmap;
} LayerImage;

void layer_image_create_append_to(LayerImage *l_t, Layer *window_layer, GRect *_bound);
void layer_image_destroy(LayerImage *l_t);