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
#define TEMPS_REVOLUTION_EARTH 365 //in days
#define TEMPS_REVOLUTION_MOON 27 //in days
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
Planet earth(ORBIT_RADIU_EARTH,TEMPS_REVOLUTION_EARTH*24);
Planet moon(ORBIT_RADIU_MOON,TEMPS_REVOLUTION_MOON*24);


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
  static bool pass30s = false;
  static bool need_refresh = false;

  timeM.update_time();

  //Each 30s : 
  if (lastT.min != timeM.min || (timeM.second>=30 && !pass30s)){
    //each minutes
    if(lastT.min != timeM.min ){
      //clear the screen
      screen.clear();
      //display the time
      screen.display_time(timeM.min, timeM.hour);
      //Calculate the position of the planets
      earth.calculatePos(timeM.day, timeM.month, timeM.hour);
      earth.addOffset(CENTER_PLANET_X,CENTER_PLANET_Y);
      moon.calculatePos(timeM.day,timeM.month, timeM.hour);
      moon.addOffset(earth.x,earth.y);
      //display the planets
      screen.display_planet(CENTER_PLANET_X,CENTER_PLANET_Y,20,ORBIT_RADIU_EARTH);
      screen.display_planet(earth.x,earth.y,10,ORBIT_RADIU_MOON);
      screen.display_planet(moon.x,moon.y,5,0);
      //need full refresh
      need_refresh = true;
    }

    //each 30S if where in a +30 situation
    if (timeM.second>=30){
      //Display the dot and register
      pass30s = true;
      //The "_P" mean that this function refresh by herself a partial part of the screen
      screen.display_dot_P();
    }else{
      //else keep the pass at 0 
      pass30s = false;
    }
  }

  //Apply the changes if needed
  if (need_refresh){
    //apply the buffer onto the screen
    screen.apply();
    need_refresh = false;
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