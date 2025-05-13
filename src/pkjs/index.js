// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config.json');
// Initialize Clay
var clay = new Clay(clayConfig);

var weatherCode = 100;
var isDay = 5;
var metric = 100;
var temperature = 999;
var ranOnce = 0;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // We will request the weather here
  // Construct URL

     if (metric == 1){
        var url = "http://api.open-meteo.com/v1/forecast?latitude=" + 
            pos.coords.latitude + "&longitude=" + pos.coords.longitude + "&current=weather_code,is_day,temperature_2m";
     } else {
      var url = "http://api.open-meteo.com/v1/forecast?latitude=" + 
          pos.coords.latitude + "&longitude=" + pos.coords.longitude + "&temperature_unit=fahrenheit&current=weather_code,is_day,temperature_2m";
     }
      // Send request to OpenWeatherMap
      xhrRequest(url, 'GET', function(responseText) {
        // responseText contains a JSON object with weather info
        var json = JSON.parse(responseText);

        if (json){

          weatherCode = 100;
          isDay = 5;
          metric = 100;
          temperature = 999;
          ranOnce = 0;

          // Conditions
          var lcl_weatherCode = json.current.weather_code;      
          weatherCode = lcl_weatherCode;
        
          // isDay
          var lcl_isDay = json.current.is_day;      
          if (lcl_isDay == 0 || lcl_isDay == 1){
            isDay = lcl_isDay;
          }

          // temperature
          var lcl_temperature = json.current.temperature_2m;      
          temperature = lcl_temperature;
          
        console.log("HELLO");
        console.log(temperature);
        ranOnce = 1;
        console.log(ranOnce);
        
          // Assemble dictionary using our keys
          var dictionary = {
            "TEMPERATURE": temperature,
            "WEATHERCODE": weatherCode,
            "ISDAY": isDay,
            "METRIC": metric,
            "RANONCE": ranOnce
          };
          // Send to Pebble
          Pebble.sendAppMessage(dictionary, function(e) {
            console.log('Weather info sent to Pebble successfully!');
            },
            function(e) {
              console.log('Error sending weather info to Pebble!');
            }
          );//sendAppMessge
        } // end json status check

      });//xhr request
      //console.log('END xhr request');  
  }; //weather check

function locationError(err) {
  console.log('Error requesting location!');
}
function getWeather() {
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
  if (!e.payload){
    return;
  }

  console.log(JSON.stringify(e.payload));
    
  var weatherCode_string;
  weatherCode_string = JSON.stringify(e.payload.WEATHERCODE);
  if (weatherCode_string) {
    weatherCode = weatherCode_string.replace(/"/g,"");
  }

  var isDay_string;
  isDay_string = JSON.stringify(e.payload.ISDAY);
  if (isDay_string) {
    isDay = isDay_string.replace(/"/g,"");
  }

  var temperature_string;
  temperature_string = JSON.stringify(e.payload.TEMPERATURE);
  if (temperature_string) {
    temperature = temperature_string.replace(/"/g,"");
  }
  
  var metric_string;
  metric_string = JSON.stringify(e.payload.METRIC);
  if (metric_string == '1'){
    metric = 1;
  } else {
    metric = 0;
  }

  var ranOnce_string;
  ranOnce_string = JSON.stringify(e.payload.RANONCE);
  if (ranOnce_string) {
    ranOnce = ranOnce_string.replace(/"/g,"");
  }
  
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
  
  metric_string = claySettings[messageKeys.METRIC];
  if (metric_string == '1'){
    metric = 1;
  } else {
    metric = 0;
  }  

  weatherCode = claySettings[messageKeys.WEATHERCODE];
  isDay = claySettings[messageKeys.ISDAY];
  temperature = claySettings[messageKeys.TEMPERATURE];
  ranOnce = claySettings[messageKeys.RANONCE];

  getWeather(); 

}
);