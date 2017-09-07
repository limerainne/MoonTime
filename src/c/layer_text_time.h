#pragma once

#include <pebble.h>

TextLayer* layer_text_time_create_append_to(Layer *window_layer);

void layer_text_time_on_update(TextLayer *s_time_layer, struct tm *tick_time);

void layer_text_date_on_update(TextLayer *s_date_layer, struct tm *tick_time);