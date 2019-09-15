#include "vec2.hpp"


Vec2::Vec2(float vX, float vY){
    x = vX;
    y = vY;    
}

Vec2::Vec2(){
    x = 0;
    y = 0;
}

std::ostream& operator << (std::ostream& stream, const Vec2& obj){
    return stream<<"vec2("<<obj.x<<","<<obj.y<<")";
}