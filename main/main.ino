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
//Daylight
#include "daylight.h"


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
Daylight daylight;

// prototypes scheduler
void each_5seconds(bool* refresh);
void each_30seconds(bool* refresh);
void each_mins(bool* refresh);
void each_hours(bool* refresh);
void each_days(bool* refresh);

const uint8_t MACAddress[] PROGMEM= {0xAC, 0x67, 0xB2, 0x2B, 0x7F, 0x20};

void setup()
{
  #if DEV
    Serial.begin(BAUDRATE);
    Serial.println("Starting setup...");

    //connect to WiFi
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  #endif
  
  //Set a fix mac adress
  esp_wifi_set_mac(WIFI_IF_STA, &MACAddress[0]);

  //Connect to wifi
  int attempt = 0;
  int cred = 0;
  int nb_credentials = sizeof(credentials)/sizeof(Credential);
  WiFi.begin(credentials[0].ssid, (credentials[0].password=="")?NULL:credentials[0].password);
  Serial.printf("\nConnecting to %s ", credentials[0].ssid);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      attempt++;
      #if DEV
        Serial.print(".");
      #endif
      if (!(attempt%15)){
        cred = (cred<nb_credentials-1)?cred+1:0;
        WiFi.begin(credentials[cred].ssid, (credentials[cred].password=="")?NULL:credentials[cred].password);
        #if DEV
          Serial.printf("\nConnecting to %s ", credentials[(attempt%nb_credentials)+1].ssid);
        #endif
      }
  }
  #if DEV
    Serial.println(" CONNECTED");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  #endif
  //init servicies
  screen.init(BAUDRATE);
  weather.init();
  
  //init time
  int i = 1;
  timeM.init();
  while(!timeM.update_time()){
    #if DEV
    Serial.println("Initilizing time, attempt "+String(i));
    #endif
    i++;
    delay(500);
  }

  init_main();
  #if DEV
    Serial.println("Setup done");
  #endif
}

void loop()
{
  static T lastT = {-1,-1,-1,-1,-1};
  static bool need_refresh = false;

  //update time
  timeM.update_time();

  //Each 30s : 
  if ((timeM.second>=30 && lastT.second<30)||(timeM.second<30 && lastT.second>=30)||(lastT.second==-1)){
    //each minutes
    if(lastT.min != timeM.min ){
      //each hours
      if(lastT.hour != timeM.hour){
        //each days
        if(lastT.day != timeM.day){
          each_days(&need_refresh);
        }
        each_hours(&need_refresh);
      }
      each_mins(&need_refresh);
    }
    each_30seconds(&need_refresh);
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
  delay(PRECISON_CLOCK);
  #else
  //ESP LIGHT SLEEP MODE (suitable for production)
  //(Slow down the Serial connection and make debug harder)
  esp_sleep_enable_timer_wakeup((PRECISON_CLOCK-100) * 1000);
  esp_light_sleep_start();
  //safe restart
  delay(100);
  #endif
}

static void init_main(void){
  static bool need_refresh = false;
  each_days(&need_refresh);
  each_hours(&need_refresh);
  each_mins(&need_refresh);
  each_30seconds(&need_refresh);
  if (need_refresh){
    //apply the buffer onto the screen
    screen.apply();
    need_refresh = false;
  }
}

/*======== SCHEDULER =========
*
*  Each function is called at a precise time
*  Here to help code reading
*/
void each_30seconds(bool* refresh){
  #if DEV
    Serial.println("--- 30 SECONDE ---");
  #endif
  //Display the dot if where at +30s
  if (timeM.second>=30)
    screen.display_dot_P(CENTER_TIME_X,BOTTOM_TIME_Y);
  //display_dot_main();
}
void each_mins(bool* refresh){
  #if DEV
    Serial.println("--- MINS ---");
  #endif
  //display the time
  screen.display_time_P(timeM.min, timeM.hour,CENTER_TIME_X,BOTTOM_TIME_Y);
}

void each_hours(bool* refresh){
  #if DEV
    Serial.println("--- HOURS ---");
  #endif
  //clear the screen
  screen.clear();

  //planets display 
  compute_pos_planet_main();
  display_planet_main();

  //update the weather data
  weather.get_info();
  screen.display_weather(weather.city, weather.temp,weather.sky,CENTER_WEATHER_X,TOP_WEATHER_Y);

  //redisplay the daylight
  display_daylight_main();

  //need full refresh
  *refresh = true;
}

void each_days(bool* refresh){
  #if DEV
    Serial.println("--- DAYS ---");
  #endif
  //update the daylight
  daylight.calculate_sunRiseSet(timeM.year,timeM.month,timeM.day,DAYLIGHT_LATITUDE,timeM.daylight_saving);
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
  screen.display_planet(CENTER_PLANET_X,CENTER_PLANET_Y,SIZE_SUN,ORBIT_RADIU_EARTH);
  screen.display_planet(earth.x,earth.y,SIZE_EARTH,ORBIT_RADIU_MOON);
  screen.display_planet(moon.x,moon.y,SIZE_MOON,0);
}
static void display_daylight_main(){
  daylight.calculate_pos(LEFT_DAYLIGHT_X,TOP_DAYLIGHT_Y,WIDTH_DAYLIGHT,HEIGHT_DAYLIGHT,timeM.hour,timeM.min);
  screen.display_suncourse(LEFT_DAYLIGHT_X,TOP_DAYLIGHT_Y,WIDTH_DAYLIGHT,HEIGHT_DAYLIGHT,daylight.sun_pos_x,daylight.sun_pos_y);
  screen.display_sun_course_hours(daylight.sunrise_hour, daylight.sunrise_min,LEFT_DAYLIGHT_X+(WIDTH_DAYLIGHT/4),TOP_DAYLIGHT_Y+HEIGHT_DAYLIGHT/2+3);
  screen.display_sun_course_hours(daylight.sunset_hour, daylight.sunset_min,LEFT_DAYLIGHT_X+(3*WIDTH_DAYLIGHT/4),TOP_DAYLIGHT_Y+HEIGHT_DAYLIGHT/2+3);
}


/*====== 30s dot handeling =======*/
static void display_dot_main(){
  //static bool dot_displayed = false;
  //each 30S if where in a +30 situation
  if (timeM.second>=30){
    //Display the dot and register
    //dot_displayed = true;
    //The "_P" mean that this function refresh by herself a partial part of the screen
    screen.display_dot_P(CENTER_TIME_X,BOTTOM_TIME_Y);
  }else{
    //else keep the pass at 0 
    //dot_displayed = false;
    screen.display_dot_P(CENTER_TIME_X,BOTTOM_TIME_Y);
  }
}