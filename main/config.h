//CONFIG
#define DEV 0
#define BAUDRATE 115200
//The smaller the more accurate but the less battery life
#define PRECISON_CLOCK 5000 //in second

/*==== Display configuration =====*/
//Define the direction of the display
// 0 & 2 = normal and reverse 
// 1 & 3 = 90° and reverse 
#define PORTRAIT_MODE 3
#define WIDTH_DISPLAY 300
#define HEIGHT_DISPLAY 400

/*==== Time widget configuration =====*/
//The time widget is a small widget that display the time
#define CENTER_TIME_X 150  //in pixels
#define BOTTOM_TIME_Y 215  //in pixels


/*==== Planet widget ==== */
//Size to display
#define SIZE_SUN 20             //in pixel
#define SIZE_EARTH 10           //in pixel
#define SIZE_MOON 5             //in pixel
#define ORBIT_RADIU_EARTH 60    //in pixel
#define ORBIT_RADIU_MOON 20     //in pixel
//Define in how many days it will do the entire rotation
#define TEMPS_REVOLUTION_EARTH 365  //in days
#define TEMPS_REVOLUTION_MOON 27    //in days
//the center of the planet widget x
#define CENTER_PLANET_X 150     //in pixels
#define CENTER_PLANET_Y 90      //in pixels
//Define the angle for the 1 of january
#define ANGLE_OFFSET M_PI_2     //in radian 


/*==== weather widget ====*/
//The weather widget is a small widget that display the weather
#define CENTER_WEATHER_X 150   //in pixels
#define TOP_WEATHER_Y 220   //in pixels
//define the URL to request
#define WEATHER_CITY "Angers"
#define WEATHER_CONTRY "FR"
#define WEATHER_APIKEY "3ca8f1a2235623063f0cd1ef6c1eec4d"
#define URL_API "http://api.openweathermap.org/data/2.5/weather?q=" WEATHER_CITY "," WEATHER_CONTRY "&units=metric&APPID=" WEATHER_APIKEY
//#define URL_API "http://api.openweathermap.org/data/2.5/weather?q=Angers,FR&units=metric&APPID=3ca8f1a2235623063f0cd1ef6c1eec4d"


/*==== Daylight widget ====*/
//The daylight widget is a small widget that display the daylight
#define LEFT_DAYLIGHT_X 0   //in pixels
#define TOP_DAYLIGHT_Y 310   //in pixels
#define WIDTH_DAYLIGHT WIDTH_DISPLAY   //in pixels
#define HEIGHT_DAYLIGHT 40   //in pixels
//define the latitude
#define DAYLIGHT_LATITUDE 47.47*100 //in rad