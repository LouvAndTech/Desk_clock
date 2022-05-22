//config
#include "config.h"

#ifndef SCREEN_H
#define SCREEN_H

class Screen{
    public:

        Screen(void);
        void init(int);
        /**
         * @brief display the time on the screen
         * 
         * @param min 
         * @param hour 
         */
        void display_time(int,int);

        /**
         * @brief Add the dot to the screen
         */
        void display_dot(void);
        
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
         * @brief Completely clear the screen
         */
        void clear();

        /**
         * @brief Apply the changes to the screen
         */
        void apply();
        void partial_apply(int,int,int,int);


    private:
        
        void print_center(const char*,int,int);

        /**
         * @brief Set the size for the font
         * 
         * @param size The size of the font 1 = smol 2 = medium 3 = big
        */
        void font_size(int);

};
#endif