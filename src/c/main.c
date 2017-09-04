#include <pebble.h>

#include "watch.h"

int main(void)    {
    on_create();
    
    app_event_loop();
    
    on_destroy();
    
    return 0;
}
