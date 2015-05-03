#include <pebble.h>
#include "effect_layer.h"
#include "main_utils.h"  
  
#define KEY_SHADOW_DIRECTION 0
#define KEY_H1_SHADOW_COLOR 1
#define KEY_H2_SHADOW_COLOR 2
#define KEY_M1_SHADOW_COLOR 3
#define KEY_M2_SHADOW_COLOR 4
#define KEY_BG_COLOR 5
#define KEY_TIME_COLOR 6  
  
#define SHADOW_DIRECTION_TO_BOTTOM_RIGHT 1
#define SHADOW_DIRECTION_TO_BOTTOM_LEFT 2
#define SHADOW_DIRECTION_TO_TOP_LEFT 3
#define SHADOW_DIRECTION_TO_TOP_RIGHT 4  

Window *my_window;
TextLayer *text_layer_hours, *text_layer_minutes, *text_layer_date, *text_layer_battery;
EffectLayer *h1, *h2, *m1, *m2;
EffectOffset oh1, oh2, om1, om2;
GColor time_color;

char s_hours[3], s_minutes[3];
char s_date[] = "Fri, Apr 24";
char buffer_battery[]="100%";

#ifndef PBL_COLOR //for Aplite - defining array that would hold set pixels (for Shadow effect)
  uint8_t *aplite_visited;
#endif
  
// changes shadows direction
void direct_shadow(uint8_t direction) {
  
  Layer *layer;
  Layer *window_layer = window_get_root_layer(my_window);
  
  #ifdef PBL_COLOR
    uint8_t length = 80;  
  #else 
    uint8_t length = 120;
  #endif
    
  switch (direction) {
    case SHADOW_DIRECTION_TO_BOTTOM_RIGHT:
      oh1.offset_x = length; oh1.offset_y = length;
      oh2.offset_x = length; oh2.offset_y = length;
      om1.offset_x = length; om1.offset_y = length;
      om2.offset_x = length; om2.offset_y = length;    
    
      layer = effect_layer_get_layer(h2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(m2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
    
      layer_set_frame(text_layer_get_layer(text_layer_date), GRect(0,-2,143, 20)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentRight);
      layer_set_frame(text_layer_get_layer(text_layer_battery), GRect(2,146,142,20)); text_layer_set_text_alignment(text_layer_battery, GTextAlignmentLeft);
    
      break;
    case SHADOW_DIRECTION_TO_BOTTOM_LEFT:
      oh1.offset_x = -length; oh1.offset_y = length;
      oh2.offset_x = -length; oh2.offset_y = length;
      om1.offset_x = -length; om1.offset_y = length;
      om2.offset_x = -length; om2.offset_y = length;    
    
      layer = effect_layer_get_layer(h1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(m1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
          
      layer_set_frame(text_layer_get_layer(text_layer_date), GRect(0,-2,143, 20)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentLeft);
      layer_set_frame(text_layer_get_layer(text_layer_battery), GRect(2,146,142,20)); text_layer_set_text_alignment(text_layer_battery, GTextAlignmentRight);
        
      break;
    case SHADOW_DIRECTION_TO_TOP_LEFT:
      oh1.offset_x = -length; oh1.offset_y = -length;
      oh2.offset_x = -length; oh2.offset_y = -length;
      om1.offset_x = -length; om1.offset_y = -length;
      om2.offset_x = -length; om2.offset_y = -length;    
    
      layer = effect_layer_get_layer(m1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(h2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(h1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
    
      layer_set_frame(text_layer_get_layer(text_layer_date), GRect(2,146,142,20)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentLeft);
      layer_set_frame(text_layer_get_layer(text_layer_battery), GRect(0,-2,143, 20)); text_layer_set_text_alignment(text_layer_battery, GTextAlignmentRight);
    
      break;
    case SHADOW_DIRECTION_TO_TOP_RIGHT:
      oh1.offset_x = length; oh1.offset_y = -length;
      oh2.offset_x = length; oh2.offset_y = -length;
      om1.offset_x = length; om1.offset_y = -length;
      om2.offset_x = length; om2.offset_y = -length;    
    
      layer = effect_layer_get_layer(h1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(h2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(m2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
    
      layer_set_frame(text_layer_get_layer(text_layer_date), GRect(2,146,142,20)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentRight);
      layer_set_frame(text_layer_get_layer(text_layer_battery), GRect(0,-2,143, 20)); text_layer_set_text_alignment(text_layer_battery, GTextAlignmentLeft);
      
      break;
  }  
    
}  
  
  
// handle configuration change
static void in_recv_handler(DictionaryIterator *iterator, void *context) {
  Tuple *t = dict_read_first(iterator);

  while (t)  {
    
    switch(t->key)    {

      #ifdef PBL_COLOR  
        case KEY_H1_SHADOW_COLOR:
           persist_write_int(KEY_H1_SHADOW_COLOR, t->value->uint8);
           oh1.offset_color = (GColor){.argb =t->value->uint8};
           break;
        case KEY_H2_SHADOW_COLOR:
           persist_write_int(KEY_H2_SHADOW_COLOR, t->value->uint8);
           oh2.offset_color = (GColor){.argb =t->value->uint8};
           text_layer_set_text_color(text_layer_date, (GColor){.argb =t->value->uint8});
           break;
        case KEY_M1_SHADOW_COLOR:
           persist_write_int(KEY_M1_SHADOW_COLOR, t->value->uint8);
           om1.offset_color = (GColor){.argb =t->value->uint8};
           text_layer_set_text_color(text_layer_battery, (GColor){.argb =t->value->uint8});
           break;
        case KEY_M2_SHADOW_COLOR:
           persist_write_int(KEY_M2_SHADOW_COLOR, t->value->uint8);
           om2.offset_color = (GColor){.argb =t->value->uint8};
           break;
         case KEY_BG_COLOR:
           persist_write_int(KEY_BG_COLOR, t->value->uint8);
           window_set_background_color(my_window,  (GColor){.argb =t->value->uint8});
         case KEY_TIME_COLOR:
           persist_write_int(KEY_TIME_COLOR, t->value->uint8);
           text_layer_set_text_color(text_layer_hours, (GColor){.argb =t->value->uint8});
           text_layer_set_text_color(text_layer_minutes, (GColor){.argb =t->value->uint8}); 
           oh1.orig_color = (GColor){.argb =t->value->uint8};
           oh2.orig_color = (GColor){.argb =t->value->uint8};
           om1.orig_color = (GColor){.argb =t->value->uint8};
           om2.orig_color = (GColor){.argb =t->value->uint8};
           break;
      #endif  
        case KEY_SHADOW_DIRECTION:
           persist_write_int(KEY_SHADOW_DIRECTION, t->value->uint8);
           direct_shadow(t->value->uint8);
     
    }    
    
    t = dict_read_next(iterator);
  
  }
  #ifndef PBL_COLOR
     memset(aplite_visited, 0, 168*20);
  #endif
  layer_mark_dirty(window_get_root_layer(my_window));
}  
  
  

static void battery_handler(BatteryChargeState state) {
  
  #ifndef PBL_COLOR
     memset(aplite_visited, 0, 168*20);
  #endif
  
  snprintf(buffer_battery, sizeof("100%"), "%d%%", state.charge_percent);
  text_layer_set_text(text_layer_battery, buffer_battery);
     
}
  



void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
   #ifndef PBL_COLOR // this mark layer as dirty so we need to reset "visited" array
     memset(aplite_visited, 0, 168*20);
   #endif
  
   if (!clock_is_24h_style()) {
    
        if( tick_time->tm_hour > 11 ) {   // YG Jun-25-2014: 0..11 - am 12..23 - pm
            if( tick_time->tm_hour > 12 ) tick_time->tm_hour -= 12;
        } else {
            if( tick_time->tm_hour == 0 ) tick_time->tm_hour = 12;
        }        
    }
  
    if (units_changed & MINUTE_UNIT) { // on minutes change - change time
      strftime(s_hours, sizeof(s_hours), "%H", tick_time);
      text_layer_set_text(text_layer_hours, s_hours);
      strftime(s_minutes, sizeof(s_hours), "%M", tick_time);
      text_layer_set_text(text_layer_minutes, s_minutes);
    }  
    
    if (units_changed & DAY_UNIT) { // on day change - change date
      strftime(s_date, sizeof(s_date), "%a, %b %d", tick_time);
      text_layer_set_text(text_layer_date, s_date);
    
    }
  
}


void handle_init(void) {
  my_window = window_create();
  #ifdef PBL_COLOR
    window_set_background_color(my_window,  persist_read_int(KEY_BG_COLOR) ? (GColor){.argb =  persist_read_int(KEY_BG_COLOR) }: GColorYellow);
  #else
    window_set_background_color(my_window, GColorBlack);
  #endif

  //creating text layers
  #ifdef PBL_COLOR
    time_color = persist_read_int(KEY_TIME_COLOR)? (GColor){.argb = persist_read_int(KEY_TIME_COLOR)} : GColorWhite;
    text_layer_hours = create_text_layer(GRect(0,16,144,70), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TYPO_GROTESK_60)), time_color, GColorClear, GTextAlignmentCenter, my_window);
    text_layer_minutes = create_text_layer(GRect(0,70,144,70), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TYPO_GROTESK_60)), time_color, GColorClear, GTextAlignmentCenter, my_window);
    text_layer_date = create_text_layer(GRect(0,-2,143,20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), persist_read_int(KEY_H2_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_H2_SHADOW_COLOR)} : GColorPurpureus, GColorClear, GTextAlignmentRight, my_window);
    text_layer_battery = create_text_layer(GRect(2,146,142,20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), persist_read_int(KEY_M1_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_M1_SHADOW_COLOR)}: GColorMayGreen, GColorClear, GTextAlignmentLeft, my_window); 
  #else
    text_layer_hours = create_text_layer(GRect(0,16,144,70), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TYPO_GROTESK_60)), GColorWhite, GColorClear, GTextAlignmentCenter, my_window);
    text_layer_minutes = create_text_layer(GRect(0,70,144,70), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TYPO_GROTESK_60)), GColorWhite, GColorClear, GTextAlignmentCenter, my_window);
    text_layer_date = create_text_layer(GRect(0,-2,143, 20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GColorWhite, GColorClear, GTextAlignmentRight, my_window);
    text_layer_battery = create_text_layer(GRect(2,146,142,20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GColorWhite, GColorClear, GTextAlignmentLeft, my_window);
  #endif
 
  
  //creating shadow layers
  #ifdef PBL_COLOR
    h1 = create_shadow_layer(GRect(10,25,51,60), time_color, persist_read_int(KEY_H1_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_H1_SHADOW_COLOR)} : GColorBrilliantRose, 70, 70, 1, &oh1, my_window);
    h2 = create_shadow_layer(GRect(64,25,51,60), time_color, persist_read_int(KEY_H2_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_H2_SHADOW_COLOR)} : GColorPurpureus, 70, 70, 1, &oh2, my_window);
    m1 = create_shadow_layer(GRect(10,85,51,60), time_color, persist_read_int(KEY_M1_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_M1_SHADOW_COLOR)}: GColorMayGreen, 70, 70, 1, &om1, my_window);
    m2 = create_shadow_layer(GRect(64,85,51,60), time_color, persist_read_int(KEY_M2_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_M2_SHADOW_COLOR)}: GColorLiberty, 70, 70, 1, &om2, my_window);
  #else
    
    // creating array for "visited" pixels and assigning it to shadow effect parameter
    aplite_visited = malloc(168*20);
    oh1.aplite_visited = aplite_visited;
    oh2.aplite_visited = aplite_visited;
    om1.aplite_visited = aplite_visited;
    om2.aplite_visited = aplite_visited;
    
    h1 = create_shadow_layer(GRect(10,25,51,60), GColorWhite, GColorWhite, 120, 120, 1, &oh1, my_window);
    h2 = create_shadow_layer(GRect(64,25,51,60), GColorWhite, GColorBlack, 120, 120, 1, &oh2, my_window);
    m1 = create_shadow_layer(GRect(10,85,51,60), GColorWhite, GColorBlack, 100, 100, 1, &om1, my_window);
    m2 = create_shadow_layer(GRect(64,85,51,60), GColorWhite, GColorWhite, 100, 100, 1, &om2, my_window);
  #endif
    
    
  direct_shadow( persist_read_int(KEY_SHADOW_DIRECTION)?  persist_read_int(KEY_SHADOW_DIRECTION) : SHADOW_DIRECTION_TO_BOTTOM_RIGHT);  
  
  window_stack_push(my_window, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
   //Get a time structure so that the face doesn't start blank
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);
 
  //Manually call the tick handler when the window is loading
  tick_handler(t, DAY_UNIT | MINUTE_UNIT);
  
  //getting battery info
  battery_state_service_subscribe(battery_handler);
  battery_handler(battery_state_service_peek());
  
  // subscribing to JS messages
  app_message_register_inbox_received(in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
}

void handle_deinit(void) {
  text_layer_destroy(text_layer_hours);
  text_layer_destroy(text_layer_minutes);
  text_layer_destroy(text_layer_date);
  
  effect_layer_destroy(h1); effect_layer_destroy(h2);
  effect_layer_destroy(m1); effect_layer_destroy(m2);
  
  #ifndef PBL_COLOR
    free(aplite_visited);
  #endif
    
  window_destroy(my_window);
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  app_message_deregister_callbacks();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
