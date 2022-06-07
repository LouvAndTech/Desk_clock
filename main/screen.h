//config
#include "config.h"
#include "Arduino.h"

#ifndef SCREEN_H
#define SCREEN_H

class Screen{
    public:

        int display_width;
        int display_height;

        Screen(void);
        void init(int);
        /**
         * @brief display the time on the screen
         * 
         * @param min 
         * @param hour 
         * @param center_x
         * @param center_y 
         */
        void display_time_P(int,int,int,int);

        /**
         * @brief Add the dot to the screen
         */
        void display_dot_P(int,int);
        
        /**
         * @brief add a planet to the screen
         * 
         * @param x pos x
         * @param y pos y
         * @param size size of the planet
         * @param orbit orbit arounded the planet 0 = none
         */
        void display_planet(int,int,int,int);

        /**
         * @brief Display the weather
         * 
         * @param City 
         * @param temp 
         * @param weather 
         * @param center_x
         * @param top_y 
         */
        void display_weather(String,int,int,int,int);

        /**
         * @brief Display the sun course
         * 
         * @param left_x
         * @param top_y
         * @param width
         * @param height
         * @param sun_x
         * @param sun_y 
         */
        void display_suncourse(int , int ,int ,int ,int,int);

        /**
         * @brief Completely clear the screen
         */
        void clear();

        /**
         * @brief Apply the changes to the screen
         */
        void apply();
        void partial_apply(int,int,int,int);

        void display_sun_course_hours(int ,int ,int ,int);

    private:
        
        void print_center(const char*,int,int);

        /**
         * @brief Set the size for the font
         * 
         * @param size The size of the font 1 = smol 2 = medium 3 = big
        */
        void font_size(int);

        void convert_hour_char(char*,int,int);

};
#endif