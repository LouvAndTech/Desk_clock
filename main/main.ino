#include <WiFi.h>
#include <esp_wifi.h>
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

const uint8_t MACAddress[] PROGMEM= {0xAC, 0x67, 0xB2, 0x2B, 0x7F, 0x20};

void setup()
{
  Serial.begin(BAUDRATE);
  Serial.println("Starting setup...");

  //connect to WiFi
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Connecting to %s ", ssid);
  esp_wifi_set_mac(WIFI_IF_STA, &MACAddress[0]);
  WiFi.begin(ssid, (password=="")?NULL:password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //init servicies
  screen.init(BAUDRATE);
  weather.init();

  //init time
  int i = 1;
  timeM.init();
  while(!timeM.update_time()){
    Serial.println("Initilizing time, attempt "+String(i));
    i++;
    delay(500);
  }

  Serial.println("Setup done");
}

void loop()
{
  static T lastT = {-1,-1,-1,-1,-1};
  static bool need_refresh = false;

  //update time
  timeM.update_time();

  //Each 30s : 
  if ((timeM.second>=30 && lastT.second<30)||(timeM.second<30 && lastT.second>=30)||(lastT.second==-1)){
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

  //display the time
  screen.display_time(timeM.min, timeM.hour);

  //(WIP) Finding a way to fully refresh only partial part of the screen to avoid redisplaying the whole thing
  //planets display 
  display_planet_main();
  //redisplay the weather
  screen.display_weather(weather.city, weather.temp,weather.sky);

  //need full refresh
  *refresh = true;
}
void each_hours(bool* refresh){
    //update the weather data
    weather.get_info();
    compute_pos_planet_main();
}
void each_days(bool* refresh){

}


/*======== FUNCTUNS TO CLARIFY THE SCHEDULER =========*/

/*====== Planet handeling ======*/
static void compute_pos_planet_main(){
  earth.calculatePos(timeM.day, timeM.month, timeM.hour,timeM.year);
  earth.addOffset(CENTER_PLANET_X,CENTER_PLANET_Y);
  moon.calculatePos(timeM.day,timeM.month, timeM.hour,timeM.year);
  moon.addOffset(earth.x,earth.y);
}
static void display_planet_main(){
  screen.display_planet(CENTER_PLANET_X,CENTER_PLANET_Y,20,ORBIT_RADIU_EARTH);
  screen.display_planet(earth.x,earth.y,10,ORBIT_RADIU_MOON);
  screen.display_planet(moon.x,moon.y,5,0);
}

/*====== 30s dot handeling =======*/
static void display_dot_main(){
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