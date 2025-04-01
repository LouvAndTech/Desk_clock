//config
#include "config.hpp"
#include "Arduino.h"
#include <cmath>

#ifndef DAULIGHT_H
#define DAULIGHT_H

class Daylight{
    public:
        int index;
        //sunrise
        int sunrise_hour;
        int sunrise_min;
        //Sunset
        int sunset_hour;
        int sunset_min;
        //pos
        int sun_pos_x;
        int sun_pos_y;

        /**
         * @brief Function to calculate the sunrise and sunset
         * 
         * @param year
         * @param month
         * @param day
         * @param latitude
         */
        void calculate_sunRiseSet(int, int, int,int,int);
        /**
         * @brief Function to update the postition of the sun in the sky
         * 
         * @param left_x
         * @param top_y
         * @param width
         * @param height
         * @param hour
         * @param min
         */
        void calculate_pos(int,int,int,int,int,int);


    private:
        int sunrise_nbMin;
        int sunset_nbMin;

        bool isLeapYear(int);
        int dayOfYear(int,int,int);
        float dTr (float);
        float rTd (float);

};
#endif