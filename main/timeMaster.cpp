//Header file
#include "timeMaster.h"
//config
#include "config.h"


//Libraries Importing
#include "time.h"
#include "sntp.h"

static const char* ntpServer1 = "pool.ntp.org";
static const char* ntpServer2 = "time.nist.gov";
static const long  gmtOffset_sec = 3600;
static const int   daylightOffset_sec = 3600;

static const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

static struct tm timeinfo;

//Constructor
TimeM::TimeM(void){
    second = 0;
    min = 0;
    hour = 0;
    day = 0;
    month = 0;
    year = 0;
    daylight_saving = 0;
}

#if DEV
// Callback function (get's called when time adjusts via NTP)
void TimeM::timeavailable(struct timeval *t)
{   
    Serial.println("Got time adjustment from NTP!");
}
#endif

//Init the connection to the NTP server
void TimeM::init(void){
    #if DEV
    // set notification call-back function
    sntp_set_time_sync_notification_cb( timeavailable );
    #endif
    /**
     * NTP server address could be aquired via DHCP,
     *
     * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
     * otherwise SNTP option 42 would be rejected by default.
     * NOTE: configTime() function call if made AFTER DHCP-client run
     * will OVERRIDE aquired NTP server address
     */
    sntp_servermode_dhcp(1);    // (optional)

    /**
     * This will set configured ntp servers and constant TimeZone/daylightOffset
     * should be OK if your time zone does not need to adjust daylightOffset twice a year,
     * in such a case time adjustment won't be handled automagicaly.
     */
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

    /**
     * A more convenient approach to handle TimeZones with daylightOffset 
     * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
     */
    //configTzTime(time_zone, ntpServer1, ntpServer2);
}

//Public methods
bool TimeM::update_time(void)
{
    bool check = getLocalTime(&timeinfo);
    if(!check){
        #if DEV
        Serial.println("No time available (yet)");
        #endif
    }else{
        day = timeinfo.tm_mday;
        hour = timeinfo.tm_hour;
        min = timeinfo.tm_min;
        second = timeinfo.tm_sec;
        month = timeinfo.tm_mon;
        year = timeinfo.tm_year;
        daylight_saving = timeinfo.tm_isdst;
        #if DEV
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        #endif
    }
    return check;
}



