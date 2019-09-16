#pragma once

#include "vec2.hpp"
#include "color.hpp"
#include <iostream>

class Line{

    static bool initialized;
    static unsigned int program;
    static unsigned int nextId;

    unsigned int id;
    Vec2 point1;
    Vec2 point2;
    Color color;
    float width;


public:
    Line(Vec2, Vec2, float, Color); 
    void draw();
    Vec2 getPos(unsigned int pointIndex);
    unsigned int getId();
    float getWidth();
    Color getColor();
    void moveTo(unsigned int pointIndex, Vec2 newPos);
}; 

std::ostream& operator << ( std::ostream&, Line obj);