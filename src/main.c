#include <pebble.h>
#include "effect_layer.h"
#include "main_utils.h"  

Window *my_window;
TextLayer *text_layer_hours, *text_layer_minutes, *text_layer_date, *text_layer_battery;
EffectLayer *h1, *h2, *m1, *m2;
EffectOffset oh1, oh2, om1, om2;

char s_hours[3], s_minutes[3];
char s_date[] = "Fri, Apr 24";
char buffer_battery[]="100%";

#ifndef PBL_COLOR //for Aplite - defining array that would hold set pixels (for Shadow effect)
  uint8_t *aplite_visited;
#endif

static void battery_handler(BatteryChargeState state) {
  
  #ifdef PBL_COLOR
   
    switch (state.charge_percent) {
       case 100: text_layer_set_text_color(text_layer_battery, GColorIslamicGreen); break;
       case 90: text_layer_set_text_color(text_layer_battery,  GColorDarkGreen); break;
       case 80: text_layer_set_text_color(text_layer_battery,  GColorCobaltBlue); break;
       case 70: text_layer_set_text_color(text_layer_battery,  GColorDukeBlue); break;
       case 60: text_layer_set_text_color(text_layer_battery,  GColorOxfordBlue); break;
       case 50: text_layer_set_text_color(text_layer_battery,  GColorImperialPurple); break;
       case 40: text_layer_set_text_color(text_layer_battery,  GColorBulgarianRose); break;
       case 30: text_layer_set_text_color(text_layer_battery,  GColorRoseVale); break;
       case 20: text_layer_set_text_color(text_layer_battery,  GColorDarkCandyAppleRed); break;
       case 10: text_layer_set_text_color(text_layer_battery,  GColorRed); break;
   }
  
  #else // this mark layer as dirty so we need to reset "visited" array"
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
    window_set_background_color(my_window, GColorYellow);
  #else
    window_set_background_color(my_window, GColorBlack);
  #endif

  //creating text layers
  text_layer_hours = create_text_layer(GRect(10,16,110,70), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TYPO_GROTESK_60)), GColorWhite, GColorClear, GTextAlignmentCenter, my_window);
  text_layer_minutes = create_text_layer(GRect(10,70,110,70), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TYPO_GROTESK_60)), GColorWhite, GColorClear, GTextAlignmentCenter, my_window);
  #ifdef PBL_COLOR
    text_layer_date = create_text_layer(GRect(60,-2,80,20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GColorLiberty, GColorClear, GTextAlignmentRight, my_window);
  #else
    text_layer_date = create_text_layer(GRect(60,-2,80,20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GColorWhite, GColorClear, GTextAlignmentRight, my_window);
  #endif
  text_layer_battery = create_text_layer(GRect(2,146,50,20), fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GColorWhite, GColorClear, GTextAlignmentLeft, my_window);
  
  //creating shadow layers
  #ifdef PBL_COLOR
    h1 = create_shadow_layer(GRect(10,25,51,60), GColorWhite, GColorBrilliantRose, 70, 70, 1, &oh1, my_window);
    h2 = create_shadow_layer(GRect(64,25,51,60), GColorWhite, GColorPurpureus, 70, 70, 1, &oh2, my_window);
    m1 = create_shadow_layer(GRect(10,85,51,60), GColorWhite, GColorMayGreen, 70, 70, 1, &om1, my_window);
    m2 = create_shadow_layer(GRect(64,85,51,60), GColorWhite, GColorLiberty, 70, 70, 1, &om2, my_window);
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
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
