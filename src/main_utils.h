#pragma once
#include <pebble.h>    
#include <pebble-effect-layer/pebble-effect-layer.h>


TextLayer* create_text_layer(GRect coords, GFont font, GColor text_color, GColor bg_color, GTextAlignment text_align, Window *window);

EffectLayer* create_shadow_layer(GRect coords, GColor orig_color, GColor shadow_color, int offset_x, int offset_y, int long_shadow, EffectOffset *props, Window *window);

EffectLayer* create_outline_layer(GRect coords, GColor orig_color, GColor outline_color, int offset_x, int offset_y, EffectOffset *props, Window *window);

GColor color_inverted(GColor source);