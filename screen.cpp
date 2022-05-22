//Header file
#include "screen.h"

//Librairies import
#include <GxEPD2_BW.h>

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
    display.init(baudrate); // enable diagnostic output on Serial
    display.setRotation(3); //Portrait mode
    display.setTextColor(GxEPD_BLACK);  //Set color to black cause that's the only one
}
void Screen::clear(){
    display.fillScreen(GxEPD_WHITE);
}
void Screen::apply(){
    display.display();
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
void Screen::display_time(int min, int hour,bool point){
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
    //The point need to be printed at the right
    if(point){display.print('.');}
}

void Screen::display_planet(int x, int y, int size, int orbit){
    display.fillCircle(x, y, size, GxEPD_BLACK);
    if (orbit){
        display.drawCircle(x, y, orbit, GxEPD_BLACK);
    }
}