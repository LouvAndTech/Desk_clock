#include <WiFi.h>
#include "time.h"
#include "sntp.h"

//config
#include "config.h"
//Screen 
#include "screen.h"
//time
#include "timeMaster.h"
//Planet calculation
#include "position.h"

#define BAUDRATE 115200
#define ORBIT_RADIU_EARTH 60 //in pixel
#define ORBIT_RADIU_MOON 20 //in pixel
#define CENTER_PLANET_X 150
#define CENTER_PLANET_Y 250

const char* ssid       = "Livebox-6858";
const char* password   = "xVdxfd4CVHGTHD82Hx";

typedef struct{
    int min;
    int hour;
    int day;
    int month;
}T;


Screen screen;
TimeM timeM;
Planet earth(ORBIT_RADIU_EARTH);
Planet moon(ORBIT_RADIU_MOON);


void setup()
{
  Serial.begin(BAUDRATE);
  Serial.println("Starting setup...");

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init servicies
  screen.init(BAUDRATE);

  timeM.init();
  while(!timeM.update_time()){
    Serial.println("Initilizing time");
    delay(500);
  }
  Serial.println("Time initilized");

  Serial.println("Setup done");
}

void loop()
{
  static T lastT = {-1,-1,-1,-1};
  static bool pass30S = false;

  timeM.update_time();
  
  //Each 30s : 
  if (lastT.min != timeM.min || (timeM.second>=30 && !pass30S)){
    
    screen.display_dot();
    pass30S = (timeM.second>=30)?true:false;

    //Each minutes
    if(lastT.min != timeM.min ){
      //cleat the screen
      screen.clear();
      //Calculate the position of the planets
      earth.calculatePos(365*24,timeM.day, timeM.month, timeM.hour);
      earth.addOffset(CENTER_PLANET_X,CENTER_PLANET_Y);
      moon.calculatePos(27*24,timeM.day,timeM.month, timeM.hour);
      moon.addOffset(earth.x,earth.y);
      //Display the planets
      screen.display_planet(CENTER_PLANET_X,CENTER_PLANET_Y,20,ORBIT_RADIU_EARTH);
      screen.display_planet(earth.x,earth.y,10,ORBIT_RADIU_MOON);
      screen.display_planet(moon.x,moon.y,5,0);

      //Display the time
      screen.display_time(timeM.min, timeM.hour);

      //apply the changes
      screen.apply();
    }
  }
  
  //update the lastT
  lastT.min = timeM.min;
  lastT.hour = timeM.hour;
  lastT.day = timeM.day;
  lastT.month = timeM.month;

  #if DEV
  //SIMPLE PROCESSOR DELAY (suitable for debug)
  delay(5000);
  #else
  //ESP LIGHT SLEEP MODE (suitable for production)
  //(Slow down the Serial connection and make debug harder)
  esp_sleep_enable_timer_wakeup(4900 * 1000);
  esp_light_sleep_start();
  //safe restart
  delay(100);
  #endif
}