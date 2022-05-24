#include "Arduino.h"
//config
#include "config.h"

#ifndef TIMEMASTER_H
#define TIMEMASTER_H

class TimeM{
    public:
        int year;
        int month;
        int day;
        int min;
        int hour;
        int second;

        TimeM(void);
        void init(void);

        bool update_time(void);

    private:
        #if DEV
        static void timeavailable(struct timeval *t);
        #endif
};
#endif