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
}

//Private Methods :
void Screen::init(int baudrate){
    #if DEV
    display.init(baudrate); // enable diagnostic output on Serial
    #else
    display.init();
    #endif
    display.setRotation(3); //Portrait mode
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

void Screen::print_center(const char *buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(x - w / 2, y);
    display.print(buf);
}

//Public Methods :
void Screen::display_time(int min, int hour){
    font_size(BIG_FONT);                // set font size
    char buffer[5];
    //Convert to string the data
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
    print_center(buffer,display.width()/2,30+24);
}
void Screen::display_dot_P(void){
    //The point need to be printed at the right of the clock
    #if DEV
    Serial.println("Display dot !");
    #endif
    font_size(BIG_FONT);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds("00:00",display.width()/2,30+24, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(display.width()/2+(w/2), 30+24);
    display.print('.');
    partial_apply(display.width()/2+(w/2), 30+24,20,20);
    display.powerOff();
}

void Screen::display_planet(int x, int y, int size, int orbit){
    display.fillCircle(x, y, size, GxEPD_BLACK);
    if (orbit){
        display.drawCircle(x, y, orbit, GxEPD_BLACK);
    }
}


void Screen::display_weather(String city, int temp, int weather){
    //Print the city
    font_size(MEDIUM_FONT);
    display.setCursor(display.width()/2, 110);
    display.print(city);
    font_size(MEDIUM_FONT);
    //Print the temperature
    display.setCursor(display.width()/2+10, 135);
    display.print(temp);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temp),display.width()/2+10, 145, &x1, &y1, &w, &h); //calc width of new string
    display.drawBitmap(display.width()/2+10+w+10,y1-10,epd_bitmap_wi_celsius,22,20,GxEPD_BLACK);
    //print the weather icon
    display.drawBitmap(50, 85, weather_icon[weather], 80,80, GxEPD_BLACK);
}