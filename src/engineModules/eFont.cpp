#include "eFont.hpp"
#include <cstdlib>
#include <stdio.h>
eFont::eFont(const char* file, int size = 35){
        sdl_font = TTF_OpenFont(file, size); 
        if(sdl_font == nullptr){
            printf("TTF_OPEN FONT: %s\n", TTF_GetError());
            std::exit(EXIT_FAILURE);
        }
}

eFont::~eFont(){
    TTF_CloseFont(sdl_font);
    sdl_font = nullptr;
}