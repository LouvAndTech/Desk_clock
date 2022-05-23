//header file
#include "weather.h"

//libraries
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define URL_API "http://api.openweathermap.org/data/2.5/weather?q=Angers,FR&units=metric&APPID=3ca8f1a2235623063f0cd1ef6c1eec4d"

Weather::Weather(void){
    city[0] = '\0';
    sky = 0;
    temp = 0;
}

void Weather::get_info(void){
    update_info();
}

void Weather::update_info(void){
  static String url = URL_API;
  static String jsonBuffer = httpGETRequest(url.c_str());
  #if DEV
  Serial.println(jsonBuffer);
  #endif
  JSONVar myObject = JSON.parse(jsonBuffer);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  #if DEV
  Serial.print("Temperature: ");
  Serial.println(myObject["main"]["temp"]);
  Serial.print("City: ");
  Serial.println(myObject["name"]);
  Serial.print("Sky: ");
  Serial.println(myObject["weather"][0]["id"]);
  #endif

  
  city =  JSON.stringify(myObject["name"]);
  city.remove(city.length()-1,1);
  city.remove(0,1);
  String skyS = JSON.stringify(myObject["weather"][0]["id"]);
  String tempS =  JSON.stringify(myObject["main"]["temp"]);

  temp = 0;
  temp += (tempS[0] - '0')*10;
  temp += (tempS[1] - '0');
  /*
  Sky[0] give :
    - 2XX thunderstorm
    - 3XX drizzle
    - 5XX rain
    - 6XX snow
    - 7XX atmosphere
    - 800 clear
    - 80X clouds
  */
  sky = skyS[0] - '0';
  /*
  If it's 8
  we convert it to the thrid element and it give :
    - 0 Clears
    - 1 Clouds
  */
  if (sky == 8) {
    if (skyS[3] == '0') {
      sky = 0;
    } else {
      sky = 1;
    }
  }else if (sky == 7) {
    sky = 1;
  }

}

String Weather::httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}