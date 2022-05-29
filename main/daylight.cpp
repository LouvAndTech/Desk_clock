#include "daylight.h"


int Daylight::dayOfYear(int day,int month,int year){
    static int dayInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    //handle leap year
    if (isLeapYear(year)){
        dayInMonth[1] = 29;
    }else{
        dayInMonth[1] = 28;
    }
    //calculate the day of the year
    int dayOfYear = 0;
    for(int i=0;i<month;i++){
        #if DEV 
            Serial.println(String(i)+" -> "+String(dayInMonth[i])); 
        #endif
        dayOfYear += dayInMonth[i];
    }
    #if DEV 
        Serial.println(String(dayOfYear)+" + "+String(day));
    #endif
    dayOfYear += day;
    #if DEV
        Serial.println("--- dayOfYear ----");
        Serial.println("Month :"+String(month)+"\nDay :"+String(day)+"\nDay of year :"+String(dayOfYear));
    #endif
    return dayOfYear;
}

bool Daylight::isLeapYear(int year){
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

//Degree to Radian
float Daylight::dTr (float degree){
    return degree*M_PI/180.;
}
//Radian to Degree
float Daylight::rTd (float rad){
    return rad*(180./M_PI);
}


void Daylight::calculate_sunRiseSet(int year, int month, int day,int latitude,int daylightSaving){
    float delta = 23.45*sin(((284+dayOfYear(day,month,year))/365.)*dTr(360.)); // RESULT IN DEGREE
    float omega =  acos(-tan(dTr(latitude/100.))*tan(dTr(delta)));
    //=== sunrise ===
    sunrise_nbMin = (12-(rTd(omega)/15.))*60;
    //Add daylight saving
    sunrise_nbMin += (daylightSaving)?120:60;
    sunrise_hour = (int)(sunrise_nbMin/60);
    sunrise_min = (int)(sunrise_nbMin%60);
    //=== sunset ===
    sunset_nbMin = (12+(rTd(omega)/15.))*60;
    //Add daylight saving
    sunset_nbMin += (daylightSaving)?120:60;
    sunset_hour = (int)(sunset_nbMin/60); 
    sunset_min = (int)(sunset_nbMin%60);

    #if DEV
        Serial.print("delta : ");
        Serial.println(delta);
        Serial.print("latitude : ");
        Serial.println(latitude/100.);
        Serial.print("omega : ");
        Serial.println(omega);
        Serial.print("sunrise :");
        Serial.println(String(sunrise_nbMin) + " minutes" + " = " + String(sunrise_hour) + "h " + String(sunrise_min) + "min");
        Serial.print("sunset :");
        Serial.println(String(sunset_nbMin) + " minutes" + " = " + String(sunset_hour) + "h " + String(sunset_min) + "min");
    #endif
}

static int fucntion_cos(int x,int height,int width){
    return (int)(height*sin(x*(2*M_PI/width)+width));
}

void Daylight::calculate_pos(int left_x,int top_y, int width, int height, int hour, int min){
    const int min_max_day = 24*60;
    int min_in_day = hour*60+min;
    int x;
    #if DEV
        Serial.println("Daylight::calculate_pos: ");
        Serial.println("min_in_day : "+String(min_in_day));
        Serial.println("sunrise_nbMin = "+String(sunrise_nbMin)+" | sunset_nbMin = "+String(sunset_nbMin));
    #endif
    if (min_in_day<sunrise_nbMin){ 
        x = (int)((min_in_day*1./sunrise_nbMin)*1.*width/4);
        #if DEV 
        Serial.print("Morning: ");
        Serial.println(x + left_x);
        #endif
    }else if (min_in_day>sunset_nbMin){
        x = (int)(((min_in_day-sunset_nbMin)*1./(min_max_day-sunset_nbMin))*1.*width/4);
        x += (3*width/4);
        #if DEV 
        Serial.print("Evening: ");
        Serial.println(x + left_x);
        #endif
    }else {
        x = (int)(((min_in_day-sunrise_nbMin)*1./(min_max_day-sunrise_nbMin))*width/2);
        x +=(width/4);
        #if DEV 
        Serial.print("Day: ");
        Serial.println(x + left_x);
        #endif
    }
    //Calculate pos
    sun_pos_x = x + left_x;
    sun_pos_y = (top_y+height) - fucntion_cos(x,height,width);
}
