#include <pebble.h>
#include <string.h>
  
#define READY 1
#define TEMPERATURE 2
#define WEATHERCODE  3
#define ISDAY 4
#define METRIC 5
#define RANONCE 6

// Define our settings struct
typedef struct ClaySettings {
  int weatherCode;
  int isDay;
  int metric;
  int temperature;
  int ranOnce;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_weather_layer;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;

// I'm making this global so that I can always get the value
static char weather_layer_buffer[32];
bool date;
int tick_counter;


// Save the settings to persistent storage
static void default_settings() {
  settings.weatherCode = 100;
  settings.isDay = 5;
  settings.metric = 100;
  settings.temperature = 999;
  settings.ranOnce = 0;
}

// Save the settings to persistent storage
static void save_settings() {
  persist_write_int(MESSAGE_KEY_WEATHERCODE, settings.weatherCode);
  persist_write_int(MESSAGE_KEY_ISDAY, settings.isDay);
  persist_write_int(MESSAGE_KEY_METRIC, settings.metric);
  persist_write_int(MESSAGE_KEY_TEMPERATURE, settings.temperature);
  persist_write_int(MESSAGE_KEY_RANONCE, settings.ranOnce);
}

// get the saved settings from persistent storage
static void get_settings() {
  default_settings();
  settings.weatherCode = persist_read_int(MESSAGE_KEY_WEATHERCODE);
  settings.isDay = persist_read_int(MESSAGE_KEY_ISDAY);
  settings.metric = persist_read_int(MESSAGE_KEY_METRIC);
  settings.temperature = persist_read_int(MESSAGE_KEY_TEMPERATURE);
  settings.ranOnce = persist_read_int(MESSAGE_KEY_RANONCE);
}

static void update_bg(char Weather){

  //Weather = 'F'; //MANUAL SET for testing various conditions
  gbitmap_destroy(s_bitmap);

  switch(Weather){    
    case 'T' : //THUNDERSTORMS
      s_bitmap = PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_THUNDERSTORMS_ROUND), 
                                   gbitmap_create_with_resource(RESOURCE_ID_THUNDERSTORMS));
      break;
    case 'L' : //LOADING
      s_bitmap = PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_LOADING_ROUND), 
                                   gbitmap_create_with_resource(RESOURCE_ID_LOADING));
      break;
    case 'Q' : //NIGHT with QLOUDS
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_CLOUDSNIGHT_ROUND), 
                                                    gbitmap_create_with_resource(RESOURCE_ID_CLOUDSNIGHT)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_CLOUDSNIGHT_BW));
      break;
    case 'N' : //NIGHT
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_NIGHT_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_NIGHT)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_NIGHT_BW));
      break;
    case 'D' : //DAY
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_DAY_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_DAY)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_DAY_BW));
      break;
    case 'C' : //CLOUDS
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_CLOUDS_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_CLOUDS)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_CLOUDS_BW));
      break;
    case 'R' : //RAIN
      s_bitmap = PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_RAIN_ROUND), 
                                   gbitmap_create_with_resource(RESOURCE_ID_RAIN));
      break;
    case 'S' : //SNOW
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_SNOW_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_SNOW)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_SNOW_BW));
      break;
    case 'F' : //FUZZY
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_HAZE_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_HAZE)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_HAZE_BW));
      break;
    default : //DEFAULT
      s_bitmap = PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_LOADING_ROUND), 
                                   gbitmap_create_with_resource(RESOURCE_ID_LOADING));
        
  }
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
}

// BEGIN Weather shenanigans
static void update_weather(DictionaryIterator *iterator, bool update_background_only) {

  // Store incoming information
  int weatherCode = 100;
  int isDay = 5;
  int metric = 100;
  int temperature = 999;
  char conditions_switch = ' ';
  int ranOnce = 0;
    
  if (!update_background_only) {
    // Read tuples for data
    Tuple *weatherCode_tuple = dict_find(iterator, WEATHERCODE);
    Tuple *isDay_tuple = dict_find(iterator, ISDAY);
    Tuple *metric_tuple = dict_find(iterator, METRIC);
    Tuple *temperature_tuple = dict_find(iterator, TEMPERATURE);
    Tuple *ranOnce_tuple = dict_find(iterator, RANONCE);

    // If temp is available, use it. We may not have the weatherCode, but at least show the temp

    if (weatherCode_tuple) {
      weatherCode = (int)weatherCode_tuple->value->int32;
    }
    if (isDay_tuple) {
      isDay =  (int)isDay_tuple->value->int32;
    }
    if (metric_tuple) {
      metric =  (int)metric_tuple->value->int32;
    }
    if (temperature_tuple) {
      temperature =  (int)temperature_tuple->value->int32;
    }
    if (ranOnce_tuple) {
      ranOnce =  (int)ranOnce_tuple->value->int32;
    }
    
    // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather isDay %d", isDay);
    // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather weatherCode %d", weatherCode);
    // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather metric %d", metric);
    // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather temperature %d", temperature);
    // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather ranOnce %d", ranOnce);
  }

  if (isDay != 1 && isDay != 0){
    isDay = settings.isDay;
  }

  if (weatherCode == 100 ){
    weatherCode = settings.weatherCode;
  }

  if (temperature == 999 ){
    temperature = settings.temperature;
  }

  if (metric == 100 ){
    metric = settings.metric;
  }

  if (ranOnce == 0 && settings.ranOnce == 1){
    ranOnce = settings.ranOnce;
  }

  if (!ranOnce){
    // APP_LOG(APP_LOG_LEVEL_INFO, "AAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHH");
    return;
  }
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather settings.isDay %d", settings.isDay);
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather settings.weatherCode %d", settings.weatherCode);
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather settings.metric %d", settings.metric);
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather settings.temperature %d", settings.temperature);
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather settings.ranOnce %d", settings.ranOnce);

  //Rain
  if (weatherCode == 51 
    || weatherCode == 53 
    || weatherCode == 55 
    || weatherCode == 56 
    || weatherCode == 57
    || weatherCode == 61
    || weatherCode == 63 
    || weatherCode == 65 
    || weatherCode == 66
    || weatherCode == 67 
    || weatherCode == 80 
    || weatherCode == 81 
    || weatherCode == 82){
      if (isDay == 0){
        conditions_switch = 'Q';
      } else {
        conditions_switch = 'R';
      }
  }  
  //ThunderStorms 
  if (weatherCode == 95 
    || weatherCode == 96 
    || weatherCode == 99){
      if (isDay == 0){
        conditions_switch = 'Q';
      } else {
        conditions_switch = 'T';
      }
  }
  //Snow
  if (weatherCode == 71 
    || weatherCode == 73 
    || weatherCode == 75 
    || weatherCode == 77
    || weatherCode == 85 
    || weatherCode == 86){
      if (isDay == 0){
        conditions_switch = 'Q';
      } else {
        conditions_switch = 'S';
      }
  }
  //Clear
  if (weatherCode == 0){
    if (isDay == 0){
      conditions_switch = 'N';
    } else {
      conditions_switch = 'D';
    }
  }
  //Clouds
  if (weatherCode == 1
    || weatherCode == 2
    || weatherCode == 3){
      if (isDay == 0){
        conditions_switch = 'Q';
      } else {
        conditions_switch = 'C';
      }
  }
  //Fuzzy Atmosphere
  if (weatherCode == 45
    || weatherCode == 48){
      if (isDay == 0){
        conditions_switch = 'Q';
      } else {
        conditions_switch = 'F';
      }
  }

  if (!conditions_switch) {
    //We didn't have anything, but we had a value for conditions
    // This would only happen if OpenWeather added a new MAIN value in the json
    // let's just default to Day, since we know that much
    if (isDay == 0){
      conditions_switch = 'N';
    } else {
      conditions_switch = 'D';
    }
  }
  
  if (weatherCode < 100){
    settings.weatherCode = weatherCode;
  }

  if (isDay == 0 || isDay == 1){
    settings.isDay = isDay;
  }

  if (metric == 0 || metric == 1){
    settings.metric = metric;
  }

  if (temperature != 999){
    settings.temperature = temperature;
  }
  
  if (ranOnce != 0){
    settings.ranOnce = ranOnce;
  }


  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather isDay %d", settings.isDay);
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather weatherCode %d", settings.weatherCode);
  //  APP_LOG(APP_LOG_LEVEL_INFO, "update_weather metric %d", metric);
  //  APP_LOG(APP_LOG_LEVEL_INFO, "update_weather settings.metric %d", settings.metric);
  // APP_LOG(APP_LOG_LEVEL_INFO, "update_weather temperature %d", settings.temperature);

  if ((settings.isDay == 0 || settings.isDay == 1) && settings.weatherCode < 100 && settings.temperature != 999 && (settings.metric == 0 || settings.metric == 1) ){

    update_bg(conditions_switch);
    if (metric == 1) {
      snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%dc", temperature);
    } else {
      snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%df", temperature);
      
    }
  

    // We're done looking at the settings returned. let's save it for future use.
    save_settings();
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
  }


}

static void send_settings_update_weather(){

  // Begin dictionary
  DictionaryIterator *iter = NULL;

  // This is to pull the settings info from cache and push it to the index.js 
  dict_write_int(iter, MESSAGE_KEY_WEATHERCODE, &settings.weatherCode, sizeof(int), true);
  dict_write_int(iter, MESSAGE_KEY_ISDAY, &settings.isDay, sizeof(int), true);
  dict_write_int(iter, MESSAGE_KEY_METRIC, &settings.metric, sizeof(int), true);
  dict_write_int(iter, MESSAGE_KEY_TEMPERATURE, &settings.temperature, sizeof(int), true);
  dict_write_int(iter, MESSAGE_KEY_RANONCE, &settings.ranOnce, sizeof(int), true);

  // Start the sync to the js
  AppMessageResult result = app_message_outbox_begin(&iter);
  

  if(result == APP_MSG_OK) {

   
    // APP_LOG(APP_LOG_LEVEL_INFO, "send_settings_update_weather isDay %d", settings.isDay);
    // APP_LOG(APP_LOG_LEVEL_INFO, "send_settings_update_weather weatherCode %d", settings.weatherCode);
    // APP_LOG(APP_LOG_LEVEL_INFO, "send_settings_update_weather metric %d", settings.metric);
    // APP_LOG(APP_LOG_LEVEL_INFO, "send_settings_update_weather temperature %d", settings.temperature);
    // APP_LOG(APP_LOG_LEVEL_INFO, "send_settings_update_weather ranOnce %d", settings.ranOnce);

    // Send this message
    result = app_message_outbox_send();

    // Check the resultW
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
    }

  } else {
    // The outbox cannot be used right now
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
  }

  save_settings();
  update_weather(iter, false);
}
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
 
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
 
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);

}
static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // We're going to overwrite the temp layer and display the date instead if they flicked once
  // then go back if they flick again


  if (date){
    // This is the second flick
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
    date = 0;
  } else{
    static char tmp_date[6];
    time_t temp = time(NULL); 
    struct tm *tick_time = localtime(&temp);
    // Getting the date
    int month = tick_time->tm_mon + 1;
    int day = tick_time->tm_mday;
    
    // I'm making an assumption here. later I can add this as an actual settings
    if (settings.metric == 1) {
      snprintf(tmp_date, sizeof(tmp_date), "%d/%d", day, month); 
    } else {
      snprintf(tmp_date, sizeof(tmp_date), "%d/%d", month, day); 
    }
  
    text_layer_set_text(s_weather_layer, tmp_date);
    date = 1;
  }
  
}
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //////////////BASIC LAYER////////////////////
  // Create the canvas Layer
  s_bitmap = PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_LOADING_ROUND), 
                               gbitmap_create_with_resource(RESOURCE_ID_LOADING));
  s_bitmap_layer = bitmap_layer_create(GRect(0,PBL_IF_ROUND_ELSE(0, 0), bounds.size.w, bounds.size.h));
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
   
  update_bg('L'); //DEFAULT LOADING screen at first
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));

  //////////////TIME LAYER////////////////////
  // Create the TextLayer with specific bounds 
  s_time_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0, -20),PBL_IF_ROUND_ELSE(125, 125), bounds.size.w, bounds.size.h));
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //////////////WEATHER LAYER////////////////////
  // Create temperature Layer		                                      # I want the temp under the time on the Round watch               
  s_weather_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0, 50), PBL_IF_ROUND_ELSE(150, 128), bounds.size.w, bounds.size.h));	
  // Style the text
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "...");
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
   
  DictionaryIterator *iter = NULL;
  update_weather(iter, true);
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  // Destroy weather elements
  text_layer_destroy(s_weather_layer);
  //Destroy the BG
  gbitmap_destroy(s_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

  update_time();
  get_settings();
  // APP_LOG(APP_LOG_LEVEL_INFO, "tick_handler isDay %d", settings.isDay);
  // APP_LOG(APP_LOG_LEVEL_INFO, "tick_handler weatherCode %d", settings.weatherCode);
  // APP_LOG(APP_LOG_LEVEL_INFO, "tick_handler metric %d", settings.metric);
  // APP_LOG(APP_LOG_LEVEL_INFO, "tick_handler temperature %d", settings.temperature);
  // APP_LOG(APP_LOG_LEVEL_INFO, "tick_handler ranOnce %d", settings.ranOnce);
  
  // Get weather update every 30 minutes
  if((tick_time->tm_min % 30 == 0) 
    || settings.weatherCode > 99 
    || (settings.isDay != 0 && settings.isDay != 1) 
    || (settings.metric != 0 && settings.metric != 1) 
    || settings.temperature == 999
    || settings.ranOnce == 0) {
    send_settings_update_weather();
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // was this just a ready signal or a "GIVE ME WEATHER I"M HUNGRY NOW!" sign
  Tuple *ready_tuple = dict_find(iterator, READY);
  
  if (!ready_tuple) {
    // otherwise, this is just a weather update for you
    update_weather(iterator, false);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "inbox_dropped_callback Message dropped! Reason: %d", (int)reason);
}

static void outbox_failed_callback(DictionaryIterator *iter,
                                      AppMessageResult reason, void *context) {
  // The message just sent failed to be delivered
  APP_LOG(APP_LOG_LEVEL_ERROR, "outbox_failed_callback Message send failed. Reason: %d", (int)reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "outbox_sent_callback Outbox send success!");
}
// END Weather shenanigans

static void init() {


  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
 
  // Get any saved storage
  get_settings();

  // Subscribe to tap events
  accel_tap_service_subscribe(accel_tap_handler);
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  window_set_background_color(s_main_window, GColorBlack);

   // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // According to Pebble Doc, it is best practice to register the AppMessage callbacks
  // before opening it. So the registration is under INIT()
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);

  date = 0;

}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}