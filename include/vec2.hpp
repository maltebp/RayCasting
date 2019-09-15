#pragma once

#include <iostream>

struct Vec2{
    float x;
    float y;
    Vec2(float vX, float vY);
    Vec2();
 } typedef Vec2;

 std::ostream& operator << (std::ostream& stream, const Vec2& obj);