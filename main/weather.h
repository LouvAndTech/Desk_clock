#include "Arduino.h"
//config
#include "config.h"

#ifndef WEATHER_H
#define WEATHER_H

class Weather{
    public:
        String city;
        int sky;
        int temp;

        Weather(void);

        void get_info(void);

    private:

        void update_info(void);
        String httpGETRequest(const char*);


};
#endif