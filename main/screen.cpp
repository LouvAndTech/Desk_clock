//Header file
#include "screen.h"
//config
#include "config.h"

//Librairies import
#include <GxEPD2_BW.h>

//Bitmaps (made with : "https://javl.github.io/image2cpp/")
#include "bitmaps.h"

//Fonts
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>


//Set font sizes
#define BIG_FONT 3
#define MEDIUM_FONT 2
#define SMALL_FONT 1

//Init display gpio pins
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW042T2

//Constructor
Screen::Screen(void){
    display_width = 0;
    display_height = 0;
}

//Private Methods :
void Screen::init(int baudrate){
    #if DEV
    display.init(baudrate); // enable diagnostic output on Serial
    #else
    display.init();
    #endif
    display.setRotation(PORTRAIT_MODE); //Portrait mode
    display_width = display.width();
    display_height = display.height();
    clear();
    apply();
    display.setTextColor(GxEPD_BLACK);  //Set color to black cause that's the only one
    font_size(BIG_FONT);

}
void Screen::clear(){
    display.fillScreen(GxEPD_WHITE);
}
void Screen::apply(){
    display.display();
}
void Screen::partial_apply(int x, int y, int w, int h){
    display.displayWindow(x,y,w,h);
}
void Screen::font_size(int size){
    switch(size){
        case 1:
            display.setFont(&FreeMonoBold9pt7b);
            break;
        case 2:
            display.setFont(&FreeMonoBold12pt7b);
            break;
        case 3:
            display.setFont(&FreeMonoBold24pt7b);
            break;
        default:
            display.setFont(&FreeMonoBold12pt7b);
            break;
    }
}

void Screen::print_center(const char *buf, int x, int y){
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(x - w / 2, y);
    display.print(buf);
}

void Screen::convert_hour_char(char* buffer,int hour,int min){
    if (hour<10){
        buffer[0]='0';
        buffer[1]=String(hour)[0];
    }else{
        buffer[0]=String(hour)[0];
        buffer[1]=String(hour)[1];
    }
    buffer[2] =':';                 
    if (min<10){
        buffer[3]='0';
        buffer[4]=String(min)[0];
    }else{
        buffer[3]=String(min)[0];
        buffer[4]=String(min)[1];
    }
}

//Public Methods :

/*===== Time widget ======*/
void Screen::display_time(int min, int hour,int center_x,int bottom_y){
    font_size(BIG_FONT);                // set font size
    char buffer[5];
    //Convert to string the clock
    convert_hour_char(buffer,hour,min);
    print_center(buffer,center_x,bottom_y);
}
void Screen::display_dot_P(int center_time_x,int bottom_time_y){
    //The point need to be printed at the right of the clock
    #if DEV
    Serial.println("Display dot !");
    #endif
    font_size(BIG_FONT);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds("00:00",center_time_x,bottom_time_y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(center_time_x+(w/2), bottom_time_y);
    display.print('.');
    partial_apply(center_time_x+(w/2), bottom_time_y,20,20);
    display.powerOff();
}

/*==== Planet widget =====*/

void Screen::display_planet(int x, int y, int size, int orbit){
    display.fillCircle(x, y, size, GxEPD_BLACK);
    if (orbit){
        display.drawCircle(x, y, orbit, GxEPD_BLACK);
    }
}

/*==== WEATHER WIDGET ====*/

void Screen::display_weather(String city, int temp, int weather, int center_x, int top_y){
    //Print the city
    font_size(MEDIUM_FONT);
    display.setCursor(center_x, top_y+25);
    display.print(city);
    font_size(MEDIUM_FONT);
    //Print the temperature
    int16_t x1, y1;
    uint16_t w, h;
    display.setCursor(center_x+10, top_y+50);
    display.print(temp);
    display.getTextBounds(String(temp),center_x+10, top_y+60, &x1, &y1, &w, &h); //calc width of new string
    display.drawBitmap(x1+w+10, top_y+30,epd_bitmap_wi_celsius,22,20,GxEPD_BLACK);
    //print the weather icon
    display.drawBitmap(center_x-100, top_y, weather_icon[weather], 80,80, GxEPD_BLACK);
}

/*==== Sunrise Sunset widget =====*/
static int fucntion_cos(int x,int height,int width){
    return (int)(height*sin(x*(2*M_PI/width)+width));
}

void Screen::display_suncourse(int left_x, int top_y,int width,int height,int sun_x,int sun_y){
    for(int i=0;i<width;i++){
        display.drawPixel(left_x+i,(top_y+height)-fucntion_cos(i,height,width),GxEPD_BLACK);
    }
    display.drawLine(left_x,top_y+height,left_x+width,top_y+height,GxEPD_BLACK);
    display.fillCircle(sun_x,sun_y,7,GxEPD_BLACK);
    //Convert to string the clock
}

void Screen::display_sun_course_hours(int hour,int min,int center_x ,int top_y){
    char buffer[5];
    convert_hour_char(buffer,hour,min);
    font_size(SMALL_FONT);
    display.drawLine(center_x,top_y-3,center_x,top_y+7,GxEPD_BLACK);
    #if DEV
    Serial.println("Display sun course hours --- buffer : "+String(buffer));
    #endif
    print_center(buffer,center_x-1,top_y-10);
}