#pragma once

#include <pebble.h>

void on_create();
void on_destroy();

void on_load_window_main(Window *window);
void on_unload_window_main(Window *window);

void on_handle_tick(struct tm *tick_time, TimeUnits units_changed);

void on_update_time();
