//CONFIG
#define DEV 1
#define BAUDRATE 115200
//The smaller the more accurate but the less battery life
#define PRECISON_CLOCK 5000 //in second


/*==== Planet widget ==== */
//Size to display
#define SIZE_SUN 20 //in pixel
#define SIZE_EARTH 10 //in pixel
#define SIZE_MOON 5 //in pixel
#define ORBIT_RADIU_EARTH 60 //in pixel
#define ORBIT_RADIU_MOON 20 //in pixel
//Define in how many days it will do the entire rotation
#define TEMPS_REVOLUTION_EARTH 365 //in days
#define TEMPS_REVOLUTION_MOON 27 //in days
//the center of the planet widget x
#define CENTER_PLANET_X 150 //in pixels
#define CENTER_PLANET_Y 290 //in pixels
//Define the angle for the 1 of january
#define ANGLE_OFFSET M_PI_2 //in radian 

/*==== weather widget ====*/
//define the URL to request
#define URL_API "http://api.openweathermap.org/data/2.5/weather?q=Angers,FR&units=metric&APPID=3ca8f1a2235623063f0cd1ef6c1eec4d"
