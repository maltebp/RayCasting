#pragma once

#include "Vec2.hpp"
#include "Color.hpp"    

class Triangle{


public:

    Vec2 p1, p2, p3;
    Color color;

    unsigned int buffer;

    Triangle();
    void draw();

};