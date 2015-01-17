#include <pebble.h>

struct event{
  char *name,
  time_t start,
  time_t end
}

static Window *window;
static TextLayer *s_time_layer;
static TextLayer *s_event_layer; // current event name
static GFont s_time_font;

/* Communication with phone/internet */
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
  // want: next event start/end, after that start/end
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/* event component */
static void update_event(){
  // update event data
}

/* watch/time component */

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
  update_event();
}

/* Window stuff/core Pebble stuff */

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //s_time_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  update_time();

  text_layer_set_background_color(s_time_layer, GColorClear);
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_WHAT_TIME_IS_IT_36));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);
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
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
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
