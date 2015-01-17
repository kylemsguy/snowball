#include <pebble.h>

static Window *window;
static TextLayer *s_time_layer;

static void update_time() {
  static char s_time[8] = "00:00";

  // get a tm struct
  time_t tmp = time(NULL);
  struct tm *tick_time = localtime(&tmp);

  if(clock_is_24h_style()) {
    // use 24 hour format
    strftime(s_time, sizeof(s_time), "%H:%M", tick_time);
  } else {
    // use 12 hour format with AM/PM
    strftime(s_time, sizeof(s_time), "%I:%M%P", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //s_time_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  update_time();

  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_DROID_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
}

static void init(void) {
  window = window_create();
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
