//header file
#include "weather.hpp"

Weather::Weather(void){
    city[0] = '\0';
    sky = 0;
    temp = 0;
}

void Weather::init(){
    //Init the weather
    get_info();
}

void Weather::get_info(void){
    update_info();
}

void Weather::update_info(void){
  JSONVar myObject;
  try{
    //Try to get a response from API and to parse it 
    myObject = formatAPI();
  }catch (...){
    //If you get an error, aboard 
    return;
  }
  #if DEV
  //Serial.print("Reponse : ");
  //Serial.println(myObject);
  #endif


  /*To consider before parsing the data
    for the weather :
      - 1 = Sunny
      - 2 = cloud 
      - 3 = thunder
      - 4 = showers
      - 5 = rain
      - 6 = snow
  */
  #if OPEN_WEATHER
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
  #endif //OPEN_WEATHER

  #if METEO_CONCEPT
    #if DEV
    Serial.print("Temperature: ");
    Serial.println(myObject["forecast"][0]["temp2m"]);
    Serial.print("City: ");
    Serial.println(myObject["city"]["name"]);
    Serial.print("Weather: ");
    Serial.println(myObject["forecast"][0]["weather"]);
    #endif

    //Treat all kind of weather to ajust to our different icons
    int weath = int(myObject["forecast"][0]["weather"]);
    WeatherIcon icon = sunny;
    if(weath<=2){ //mostly Sunny
      icon = sunny;
    }else if(weath < 10){ //kinds of clouds
      icon = cloudy;
    }else if (weath < 20){ //kinds of rains
      icon = rain;
    }else if (weath < 40){ // Snow up to 30 and rain with snow up to 40
      icon = snow;
    }else if (weath < 100){ //All kinds of showers (i don't want to treat them all with if statement... need a better solution for the future)
      icon = showers;
    }else if (weath < 210){ //All kind of storm-like weather (same as above)
      icon = thunder;
    }else{
      int r = weath - 200;
      if (r < 20){
        icon = rain;
      }else if (r < 30){
        icon = snow;
      }else{
        icon = rain;
      }
    }
    sky = (int) icon;
    
    //Treat the temp and the city:
    city = JSON.stringify(myObject["city"]["name"]);
    city.remove(city.length()-1,1);
    city.remove(0,1);
    temp = int(myObject["forecast"][0]["temp2m"]);
    

    #if DEV
    Serial.print("weath: ");
    Serial.print(weath);
    Serial.print(" / icon: ");
    Serial.print(icon);
    Serial.print(" / sky: ");
    Serial.println(sky);
    #endif
  #endif
}

String Weather::httpGETRequest(const char* serverName) {
  #if OPEN_WEATHER
    WiFiClient client;
  #endif
  #if METEO_CONCEPT
    WiFiClientSecure client;
    client.setInsecure();
  #endif
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  
  http.begin(client, serverName);
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();

  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    #if DEV
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    #endif
    payload = http.getString();
  }
  else {
    #if DEV
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    #endif
  }
  // Free resources
  http.end();

  return payload;
}

JSONVar Weather::formatAPI (void){
  static String url = URL_API;
  static String jsonBuffer = httpGETRequest(url.c_str());
  JSONVar myObject = JSON.parse(jsonBuffer);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    #if DEV
      Serial.println("Parsing input failed!");
    #endif;
  }
  return myObject;
}
