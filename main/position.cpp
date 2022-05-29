//header file
#include "position.h"
//config
#include "config.h"

//Constructor
Planet::Planet(int orbitRadius,int nbHourMax){
    r = orbitRadius;
    tempsRevolution = nbHourMax;
    x = 0;
    y = 0;
    angle = 0;
}

void Planet::calculatePos(int day, int month,int hour,int year){
    #if DEV
    Serial.println("Calculating position...");
    #endif
    //calculate the day of the year
    int doy = dayOfYear(day,month,year);
    //Calculate the angle with a *100 multiplier
    angle = (int)((((((doy*24+hour)%tempsRevolution)*1./tempsRevolution)*2.*M_PI)+ANGLE_OFFSET)*100);
    //Calculate the x and y
    x = (int)(r*cos(angle/100.));
    y = (int)(r*sin(angle/100.));
    #if DEV
    Serial.println("angle: "+String(angle/100.));
    Serial.println("x: "+String(x));
    Serial.println("y: "+String(y));
    Serial.println("Calution done !");
    #endif
}

bool Planet::isLeapYear(int year){
    if(year%4==0){
        if(year%100==0){
            if(year%400==0){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }
}

int Planet::dayOfYear(int day,int month,int year){
    static int dayInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    //handle leap year
    if (month==2 && isLeapYear(year)){
        dayInMonth[1] = 29;
    }else{
        dayInMonth[1] = 28;
    }
    //calculate the day of the year
    int dayOfYear = 0;
    for(int i=0;i<month-1;i++){
        dayOfYear += dayInMonth[i];
    }
    dayOfYear += day;
    return dayOfYear;
}

void Planet::addOffset(int xOffset, int yOffset){
    x += xOffset;
    y += yOffset;
}