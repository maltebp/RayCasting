#pragma once

#include <iostream>

struct Vec2{
    float x;
    float y;
 } typedef Vec2;

 std::ostream& operator << (std::ostream& stream, const Vec2& obj);