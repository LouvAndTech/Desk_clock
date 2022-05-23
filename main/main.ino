#include <WiFi.h>
#include "time.h"
#include "sntp.h"

//config
#include "config.h"
//wifi credential
#include "credential.h"
//Screen 
#include "screen.h"
//time
#include "timeMaster.h"
//Planet calculation
#include "position.h"
//weather
#include "weather.h"

#define BAUDRATE 115200
#define ORBIT_RADIU_EARTH 60 //in pixel
#define ORBIT_RADIU_MOON 20 //in pixel
#define TEMPS_REVOLUTION_EARTH 365 //in days
#define TEMPS_REVOLUTION_MOON 27 //in days
#define CENTER_PLANET_X 150
#define CENTER_PLANET_Y 290


//Save the last Time we pass the boucle
typedef struct{
  int second;
  int min;
  int hour;
  int day;
  int month;
}T;
void updateLastTime(T* lt,TimeM* t){
  lt->second = t->second;
  lt->min = t->min;
  lt->hour = t->hour;
  lt->day = t->day;
  lt->month = t->month;
}

//Init object
Screen screen;
TimeM timeM;
Planet earth(ORBIT_RADIU_EARTH,TEMPS_REVOLUTION_EARTH*24);
Planet moon(ORBIT_RADIU_MOON,TEMPS_REVOLUTION_MOON*24);
Weather weather;

// prototypes scheduler
void each_5seconds(bool* refresh);
void each_30seconds(bool* refresh);
void each_mins(bool* refresh);
void each_hours(bool* refresh);
void each_days(bool* refresh);


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
  static T lastT = {-1,-1,-1,-1,-1};
  static bool need_refresh = false;

  //update time
  timeM.update_time();

  //Each 30s : 
  if (lastT.min != timeM.min || (timeM.second>=30 && lastT.second<30)){
    each_30seconds(&need_refresh);
    //each minutes
    if(lastT.min != timeM.min ){
      each_mins(&need_refresh);
      //each hours
      if(lastT.hour != timeM.hour){
        each_hours(&need_refresh);
        //each days
        if(lastT.day != timeM.day){
          each_days(&need_refresh);
        }
      }
    }
  }

  //Apply the changes if needed
  if (need_refresh){
    //apply the buffer onto the screen
    screen.apply();
    need_refresh = false;
  }
  
  //update the lastT
  updateLastTime(&lastT,&timeM);

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


/*======== SCHEDULER =========
*
*  Each function is called at a precise time
*  Here to help code reading
*/
void each_30seconds(bool* refresh){
  //Display the dot if where at +30s
  display_dot_main();
}
void each_mins(bool* refresh){
  //clear the screen
  screen.clear();

  //redisplay the weather after each clear (WIP)
  screen.display_weather(weather.city, weather.temp,weather.sky);
  
  //display the time
  screen.display_time(timeM.min, timeM.hour);

  //planets
  compute_pos_planet_main();
  display_planet_main();

  //need full refresh
  *refresh = true;
}
void each_hours(bool* refresh){
    //update the weather data
    weather.get_info();
}
void each_days(bool* refresh){

}


/*======== FUNCTUNS TO CLARIFY THE SCHEDULER =========*/

/*====== Planet handeling ======*/
void compute_pos_planet_main(){
  earth.calculatePos(timeM.day, timeM.month, timeM.hour);
  earth.addOffset(CENTER_PLANET_X,CENTER_PLANET_Y);
  moon.calculatePos(timeM.day,timeM.month, timeM.hour);
  moon.addOffset(earth.x,earth.y);
}
void display_planet_main(){
  screen.display_planet(CENTER_PLANET_X,CENTER_PLANET_Y,20,ORBIT_RADIU_EARTH);
  screen.display_planet(earth.x,earth.y,10,ORBIT_RADIU_MOON);
  screen.display_planet(moon.x,moon.y,5,0);
}

/*====== 30s dot handeling =======*/
void display_dot_main(){
  static bool dot_displayed = false;
  //each 30S if where in a +30 situation
  if (timeM.second>=30){
    //Display the dot and register
    dot_displayed = true;
    //The "_P" mean that this function refresh by herself a partial part of the screen
    screen.display_dot_P();
  }else{
    //else keep the pass at 0 
    dot_displayed = false;
  }
}