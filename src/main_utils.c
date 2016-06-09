#include <pebble.h>
#include "main_utils.h"
#include <pebble-effect-layer/pebble-effect-layer.h>
  
  
GColor color_inverted(GColor source) {
    GColor inverted = source;
  if(gcolor_equal(source, GColorBlack)) 
    inverted= GColorWhite;
  if(gcolor_equal(source, GColorWhite))
    inverted= GColorBlack;
  #ifdef PBL_COLOR
    if(!gcolor_equal(source, GColorClear)) //GColorClear should not change
      inverted.argb= source.argb ^ 0b00111111;
  #endif
  return inverted;
}   
  

TextLayer* create_text_layer(GRect coords, GFont font, GColor text_color, GColor bg_color, GTextAlignment text_align, Window *window) {
  TextLayer *text_layer = text_layer_create(coords);
  text_layer_set_font(text_layer, font);
  text_layer_set_text_color(text_layer, text_color);  
  text_layer_set_background_color(text_layer, bg_color);  
  text_layer_set_text_alignment(text_layer, text_align);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  return text_layer;
}


EffectLayer* create_shadow_layer(GRect coords, GColor orig_color, GColor shadow_color, int offset_x, int offset_y, int long_shadow, EffectOffset *props, Window *window){
  
  EffectLayer *shadow_layer = effect_layer_create(coords);
  
  props->orig_color = orig_color;
  props->offset_color = shadow_color;
  props->offset_x = offset_x;
  props->offset_y = offset_y;
  props->option = long_shadow;
  
  effect_layer_add_effect(shadow_layer, effect_shadow, props);
  
  layer_add_child(window_get_root_layer(window), effect_layer_get_layer(shadow_layer));
  
  return shadow_layer;
  
}


EffectLayer* create_outline_layer(GRect coords, GColor orig_color, GColor outline_color, int offset_x, int offset_y, EffectOffset *props, Window *window){
  
  EffectLayer *shadow_layer = effect_layer_create(coords);
  
  props->orig_color = orig_color;
  props->offset_color = outline_color;
  props->offset_x = offset_x;
  props->offset_y = offset_y;
  
  effect_layer_add_effect(shadow_layer, effect_outline, props);
  
  layer_add_child(window_get_root_layer(window), effect_layer_get_layer(shadow_layer));
  
  return shadow_layer;
  
}