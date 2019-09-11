#include "vec2.hpp"


std::ostream& operator << (std::ostream& stream, const Vec2& obj){
    return stream<<"vec2("<<obj.x<<","<<obj.y<<")";
}