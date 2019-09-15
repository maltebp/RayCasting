#pragma once

#include <iostream>

struct Color{
    float r;
    float g;
    float b;
    float a;

    Color(float red, float green, float blue, float alpha);
    Color(float red, float green, float blue);
    Color();
    
} typedef Color;


//https://stackoverflow.com/questions/47083328/operator-overloading-in-header-files-and-in-the-cpp-files
std::ostream& operator << (std::ostream& stream, const Color& obj);