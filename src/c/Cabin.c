#include <pebble.h>
#include <string.h>
  
#define TEMPERATURE 0
#define CONDITIONS  1
#define SUNRISE 2
#define SUNSET 3
#define API 4
#define METRIC 5
#define READY 6
#define TRUE 1
#define FALSE 0

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  char metric[25];
  char api[255];
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
  snprintf(settings.metric, sizeof(settings.metric), "%s", "FALSE");
  snprintf(settings.api, sizeof(settings.api), "%s", "");
}

// Save the settings to persistent storage
static void save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// get the saved settings from persistent storage
static void get_settings() {
  default_settings();
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}
static void send_settings_update_weather(){

  // Begin dictionary
  DictionaryIterator *iter;

  // Start the sync to the js
  AppMessageResult result = app_message_outbox_begin(&iter);

  if(result == APP_MSG_OK) {

    // This is to pull the settings info from cache and push it to the index.js 
    dict_write_cstring(iter, MESSAGE_KEY_API, settings.api);
    dict_write_cstring(iter, MESSAGE_KEY_METRIC, settings.metric);
   
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
    date = FALSE;
  } else{
    static char tmp_date[6];
    time_t temp = time(NULL); 
    struct tm *tick_time = localtime(&temp);
    // Getting the date
    int month = tick_time->tm_mon + 1;
    int day = tick_time->tm_mday;
    
    // I'm making an assumption here. later I can add this as an actual settings
    if (strstr(settings.metric,"TRUE") != NULL) {
      snprintf(tmp_date, sizeof(tmp_date), "%d/%d", day, month); 
    } else {
      snprintf(tmp_date, sizeof(tmp_date), "%d/%d", month, day); 
    }
  
    text_layer_set_text(s_weather_layer, tmp_date);
    date = TRUE;
  }
  
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
    case 'O' : //TORNADO
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_TORNADO_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_TORNADO)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_TORNADO));
      break;
    case 'U' : //SUNRISE
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_SUNRISE_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_SUNRISE)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_SUNSET_BW));
      break;
    case 'E' : //SUNSET
      s_bitmap = PBL_IF_COLOR_ELSE(PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_SUNSET_ROUND), 
                                                     gbitmap_create_with_resource(RESOURCE_ID_SUNSET)), 
                                   gbitmap_create_with_resource(RESOURCE_ID_SUNSET_BW));
      break;
    default : //DEFAULT
      s_bitmap = PBL_IF_ROUND_ELSE(gbitmap_create_with_resource(RESOURCE_ID_LOADING_ROUND), 
                                   gbitmap_create_with_resource(RESOURCE_ID_LOADING));
        
  }
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
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
 
  // Get weather update every 30 minutes
  if((tick_time->tm_min % 30 == 0)) {
    send_settings_update_weather();
  }
}
// BEGIN Weather shenanigans
static void update_weather(DictionaryIterator *iterator) {
  // Store incoming information
  static char temperature_buffer[10];
  static char conditions_buffer[32];
  static char sunset_buffer[32];
  static char sunrise_buffer[32];

  int current_time = 0;
  int sunrise_time = 0;
  int sunset_time = 0;
  char conditions_switch = ' ';
  
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, CONDITIONS);
  Tuple *sunrise_tuple = dict_find(iterator, SUNRISE);
  Tuple *sunset_tuple = dict_find(iterator, SUNSET);
  Tuple *api_tuple = dict_find(iterator, API);
  Tuple *metric_tuple = dict_find(iterator, METRIC);

  if (api_tuple){
    snprintf(settings.api, sizeof(settings.api), "%s", api_tuple->value->cstring);
  } 

  //Save these settings
  if (metric_tuple){
    snprintf(settings.metric, sizeof(settings.metric), "%s", metric_tuple->value->cstring);
  }

  // We're done looking at the settings returned. let's save it for future use.
  //HEL TO DO, does this get over written when nothing is returned for those values?
  save_settings();
  // If temp is available, use it. We may not have the conditions, but at least show the temp
  if (temp_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d", (int)temp_tuple->value->int32);
  }

  if (conditions_tuple) {
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring); 
  }

  if (sunrise_tuple) {
    snprintf(sunrise_buffer, sizeof(sunrise_buffer), "%d", (int)sunrise_tuple->value->int32);
    sunrise_time =  (int)sunrise_tuple->value->int32;
  }
  if (sunset_tuple) {
    snprintf(sunset_buffer, sizeof(sunset_buffer), "%d", (int)sunset_tuple->value->int32);
    sunset_time =  (int)sunset_tuple->value->int32;
  }

  current_time = (int)time(NULL);

  if ((sunrise_time - (60*30)) < current_time && current_time < (sunrise_time + (60*30))){
    conditions_switch = 'U';
  }

  if ((sunset_time - (60*30)) < current_time && current_time < (sunset_time + (60*30))){
    conditions_switch = 'E';
  }

  //If we're day, but the conditions didn't return correctly we'll have a blank screen
  if ((sunrise_time < current_time && current_time < sunset_time)
      && (conditions_switch != 'E' && conditions_switch != 'U')){

    // string search conditions_buffer for key words, but only if there's a value...
    if (strlen(conditions_buffer)>0){
      //Rain
      if ((strstr(conditions_buffer,"Rain")) != NULL){  
        conditions_switch = 'R';
      }  
      //ThunderStorms 
      if ((strstr(conditions_buffer,"ThunderStorms")) != NULL){
        conditions_switch = 'T';
      }
      //Snow
      if ((strstr(conditions_buffer,"Snow")) != NULL){
        conditions_switch = 'S';
      }
      //Clear
      if ((strstr(conditions_buffer,"Clear")) != NULL){
        conditions_switch = 'D';
      }
      //Clouds
      if ((strstr(conditions_buffer,"Clouds")) != NULL){
        conditions_switch = 'C';
      }
      //Fuzzy Atmosphere
      if (((strstr(conditions_buffer,"Mist")) != NULL)
          || ((strstr(conditions_buffer,"Smoke")) != NULL)
          || ((strstr(conditions_buffer,"Haze")) != NULL)
          || ((strstr(conditions_buffer,"Dust")) != NULL)
          || ((strstr(conditions_buffer,"Fog")) != NULL)
          || ((strstr(conditions_buffer,"Sand")) != NULL)
          || ((strstr(conditions_buffer,"Dust")) != NULL)
          || ((strstr(conditions_buffer,"Ash")) != NULL)
          || ((strstr(conditions_buffer,"Squal")) != NULL))
      {
        conditions_switch = 'F';
      }
      //Tornado
      if ((strstr(conditions_buffer,"Tornado")) != NULL)
      {
        conditions_switch = 'O';
      }
      if (!conditions_switch) {
        //We didn't have anything, but we had a value for conditions
        // This would only happen if OpenWeather added a new MAIN value in the json
        // let's just default to Day, since we know that much
        conditions_switch = 'D';
      }
    // We don't know the conditions, but we know it'a day. We'll just show day.
    } else {
      conditions_switch = 'D';
    }
  // NIGHT TIME!
  } else {
    // if the time is not during the day we'll check to see if the night is clear or not. 
    // If it's NOT clear, it will be snowy/rainy/cloudy which CANNOT be differentiated in the dark
    // Trust me, I looked. I can only tell when the sky is clear or not clear by the moon 
    //    and whether I'm getting rained on
    // so we'll show the same image for all except for clear
    if  (conditions_switch != 'E' && conditions_switch != 'U'){
      if ((strstr(conditions_buffer,"Clear")) != NULL){
        conditions_switch = 'N';
      } else {
        conditions_switch = 'Q';
      }
    }
    if (conditions_switch == ' '){
      conditions_switch = 'X';
    }
  }
  
  update_bg(conditions_switch);

  if (strstr(settings.metric,"TRUE") != NULL) {
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%sc", temperature_buffer);
  } else {
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%sf", temperature_buffer);
  }

  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // was this just a ready signal or a "GIVE ME WEATHER I"M HUNGRY NOW!" sign
  Tuple *ready_tuple = dict_find(iterator, READY);

  if (ready_tuple) {
    
    // This is just a ready signal, We'll go back to the JS program
    // and give it the API information
    send_settings_update_weather();
  } else {
    // otherwise, this is just a weather update for you
    update_weather(iterator);
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

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
 
  // Subscribe to tap events
  accel_tap_service_subscribe(accel_tap_handler);
  // Get any saved storage
  get_settings();
  
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

  date = FALSE;

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