// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config.json');
// Initialize Clay
var clay = new Clay(clayConfig);

var api = "";
var metric = "";
var weather = "";

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  var temperature;
  var conditions;
  var sunset;
  var sunrise;
  console.log('in locationSuccess');
  // We will request the weather here
  // Construct URL
  
  console.log('locationSuccess api ' + api );
  console.log('locationSuccess metric ' + metric); 
  console.log('locationSuccess weather ' + weather); 

  // Don't bother with doing ANYTHING if they don't actually want the weather
  if (weather = "TRUE") {
    // Don't bother with anything unless you have an API key
    if (api) {
      if (metric == "TRUE"){
        var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + 
            pos.coords.latitude + "&lon=" + pos.coords.longitude + "&appid=" + api + "&units=METRIC";
      } else {
        var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + 
            pos.coords.latitude + "&lon=" + pos.coords.longitude + "&appid=" + api + "&units=IMPERIAL";
      }
      console.log('url to send ' + url );
      // Send request to OpenWeatherMap
      xhrRequest(url, 'GET', function(responseText) {
        // responseText contains a JSON object with weather info
        var json = JSON.parse(responseText);
        console.log('json.cod is ' + json.cod);

        if (json.cod != 200){
          //We have an error, so nothing
        } else {

          // Temperature
          temperature = json.main.temp;
          console.log('Temperature is ' + temperature);

          // Conditions
          conditions = json.weather[0].main;      
          console.log('Conditions are ' + conditions);
        
          // sunset
          sunset = json.sys.sunset;      
          console.log('sunset is ' + sunset);
        
          // sunrise
          sunrise = json.sys.sunrise;       
          console.log('sunrise is ' + sunrise);
          
          console.log('before dictionary send api ' + api );
          console.log('before dictionary send metric ' + metric); 
          console.log('before dictionary send weather ' + weather); 
        
          // Assemble dictionary using our keys
          var dictionary = {
            "TEMPERATURE": temperature,
            "CONDITIONS": conditions,
            "SUNRISE": sunrise,
            "SUNSET": sunset,
            "API": api,
            "METRIC": metric,
            "WEATHER": weather
          };
          // Send to Pebble
          Pebble.sendAppMessage(dictionary, function(e) {
            console.log('Weather info sent to Pebble successfully!');
            },
            function(e) {
              console.log('Error sending weather info to Pebble!');
            }
          );//sendAppMessge
          console.log('END sendAppMessge'); 

        } // end json status check

      });//xhr request
      //console.log('END xhr request'); 
    }; // API check
    //console.log('END API check'); 
  }; //weather check
  console.log('END WEATHER check');
}

function locationError(err) {
  
  //console.log('in locationSuccess');
  console.log('Error requesting location!');
}
function getWeather() {
  console.log('in getWeather()!');
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {

    //console.log('PebbleKit JS ready!');

    var dictionary = {
      "READY": 1
    };

    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log('READY');
      },
      function(e) {
        console.log('OH NO');
      }
     
    );  }
);
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e) {

  console.log("appmessage payload "+ JSON.stringify(e.payload));

  if (!e.payload){
    console.log("Nothiing in the payload "+ JSON.stringify(e.payload));
    return;
  }

  var api_string;
  var metric_string;
  var weather_string;
  
  api_string = JSON.stringify(e.payload.API);
  if (api_string) {
    api = api_string.replace(/"/g,"");
    //console.log('addEventListener json api ' + api)
  }
  
  metric_string = JSON.stringify(e.payload.METRIC);
  console.log('addEventListener json metric ' + metric_string)
  if (metric_string == '1'){
    metric = 'TRUE';
  } else {
    metric = 'FALSE';
  }  

  weather_string = JSON.stringify(e.payload.WEATHER);
  console.log('addEventListener json weather_string ' + weather_string)
  if (weather_string == '1'){
    weather = 'TRUE';
  } else {
    weather = 'FALSE';
  }  

  //console.log('AppMessage received!');
  getWeather();

  }                     
);
var messageKeys = require('message_keys');

Pebble.addEventListener('webviewclosed', function(e) {
  
  //console.log('e.response ' + e.response);
  if (e && !e.response) {
    return;
  };
  
  // Get the keys and values from each config item
  var claySettings = clay.getSettings(e.response);
  var metric_string;
  var weather_string;
  
  metric_string = claySettings[messageKeys.METRIC];
  if (metric_string == '1'){
    metric = 'TRUE';
  } else {
    metric = 'FALSE';
  }  
  console.log('webviewclosed metric ' + metric);

  api = claySettings[messageKeys.API];
  console.log('webviewclosed api ' + api);

  weather_string = claySettings[messageKeys.WEATHER];
  if (weather_string == '1'){
    weather = 'TRUE';
  } else {
    weather = 'FALSE';
  }  
  console.log('webviewclosed weather ' + weather);

  getWeather(); 

}
);