//#include "flashlight.h"
#include <pebble.h>

static void availability_handler(SmartstrapServiceId s, bool is_available);
static Window *window;
static TextLayer *text_layer;

  
static void lights_on()
  {
  smartstrap_subscribe((SmartstrapHandlers) {
    .availability_did_change = availability_handler,
    .did_read = NULL,
    .did_write = NULL,
    .notified = NULL
  });
}

static void lights_off()
  {
   smartstrap_unsubscribe();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  lights_on();
  text_layer_set_text(text_layer, "Light On");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  lights_off();
  text_layer_set_text(text_layer, "Light Off");
}

static void availability_handler(SmartstrapServiceId s, bool is_available) {

}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press Up to turn on");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  lights_off();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
