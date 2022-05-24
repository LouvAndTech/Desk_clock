#include "Arduino.h"
#include <cmath>
//config
#include "config.h"

#ifndef POSITION_H
#define POSITION_H

class Planet{
    public:
        int x; //largeur
        int y; //hauteur

        Planet(int,int);
        /**
         * @brief Calculates the position of the planet
         * 
         * @param nbHourMax  Number of hours to do a full revolution
         * @param day        Day of the month
         * @param month      Month of the year
         * @param hour       Hour of the day
         */
        void calculatePos(int,int,int,int);
        void addOffset(int,int);

    private:
        int angle; // Angle with a *100 multiplier
        int r; // The radius of the orbit
        int tempsRevolution; //

        bool isLeapYear(int);
        int dayOfYear(int,int,int);
        
        

};
#endif