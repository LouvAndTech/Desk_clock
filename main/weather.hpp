#include "Arduino.h"
//config
#include "config.hpp"

//libraries
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#ifndef WEATHER_H
#define WEATHER_H

typedef enum{
  sunny = 0,
  cloudy,
  thunder,
  showers,
  rain,
  snow,
}WeatherIcon;

class Weather{
    public:
      String city;
      int sky;
      int temp;

      Weather(void);
      void init(void);
      void get_info(void);

    private:

      void update_info(void);
      String httpGETRequest(const char*);
      JSONVar formatAPI(void);


};
#endif