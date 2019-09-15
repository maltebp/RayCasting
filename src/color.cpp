
#include "color.hpp"

Color::Color(float red, float green,  float blue, float alpha){
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

Color::Color(float red, float green,  float blue){
    r = red;
    g = green;
    b = blue;
    a = 1;
}

Color::Color(){
    r = 1;
    g = 1;
    b = 1;
    a = 1;
}

std::ostream& operator << (std::ostream& stream, const Color& obj){
    return stream<<"Color("<<obj.r<<","<<obj.g<<","<<obj.b<<","<<obj.a<<")";
}

