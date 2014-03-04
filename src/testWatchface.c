/*

"Classic" Digital Watch Pebble App

*/

// Standard includes
#include "pebble.h"


// App-specific data
Window *window; // All apps must have at least one window
TextLayer *time_layer; // The clock
TextLayer *date_layer;
TextLayer *day1_layer;

static GBitmap *background_image;  // declare background image
static BitmapLayer *background_layer; // background image layer

// Called once per second
static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {

  static char time_text[] = "00:00"; // Needs to be static because it's used by the system later.


  strftime(time_text, sizeof(time_text), "%l:%M", tick_time);
  text_layer_set_text(time_layer, time_text);
}


// Handle the start-up of the app
static void do_init(void) {

  // Create our app's base window
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, GColorWhite);
  
  Layer *window_layer = window_get_root_layer(window);

  background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  background_layer = bitmap_layer_create(layer_get_frame(window_layer));
  bitmap_layer_set_bitmap(background_layer, background_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));


  
  // Init the text layer used to show the time
  time_layer = text_layer_create(GRect(5, 9, 142-5, 56-9 /* height */));
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD));

  date_layer = text_layer_create(GRect(5, 112, 142-5, 128-112));
  text_layer_set_text_color(date_layer, GColorBlack);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(date_layer));
  
  day1_layer = text_layer_create(GRect(10, 125, 22-10, 174-130));
  text_layer_set_text_color(day1_layer, GColorBlack);
  text_layer_set_text_alignment(day1_layer, GTextAlignmentLeft);
  text_layer_set_background_color(day1_layer, GColorClear);
  text_layer_set_font(day1_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(day1_layer));
  
  
  text_layer_set_text(date_layer, "Su Mo Tu We Th Fr Sa");
  text_layer_set_text(day1_layer, "01");

  
  
  
  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_minute_tick(current_time, MINUTE_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
}

static void do_deinit(void) {
  // destroy background
  layer_remove_from_parent(bitmap_layer_get_layer(background_layer));
  bitmap_layer_destroy(background_layer);
  gbitmap_destroy(background_image);
  
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(day1_layer);
  window_destroy(window);
  
  
  
  
}

// The main event/run loop for our app
int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}