#include "layer_image.h"

void layer_image_create_append_to(LayerImage *l_t, Layer *window_layer, int res_id, GRect bounds)    {    
    // draw image
    l_t->s_layer = bitmap_layer_create(bounds);
    l_t->s_bitmap = gbitmap_create_with_resource(res_id);
    	
    bitmap_layer_set_compositing_mode(l_t->s_layer, GCompOpAssign);
    bitmap_layer_set_bitmap(l_t->s_layer, l_t->s_bitmap);
    bitmap_layer_set_alignment(l_t->s_layer, GAlignCenter);
    
    // add to layer
    layer_add_child(window_layer, bitmap_layer_get_layer(l_t->s_layer));
}

void layer_image_destroy(LayerImage *l_t)    {
    gbitmap_destroy(l_t->s_bitmap);
    bitmap_layer_destroy(l_t->s_layer);
}