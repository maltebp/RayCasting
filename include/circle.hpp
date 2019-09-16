#pragma once

#include "vec2.hpp"
#include "color.hpp"

#include <iostream>

class Circle{
    static bool initialized;
    static unsigned int program;
    static unsigned int nextId;
    int id;
    float radius;
    Vec2 pos;
    Color color;
    float vertices[24];
    unsigned int indices[6];
    unsigned int ibo;
    unsigned int buffer;

    void updateVerticesPos();
    void updateVerticesCol();

public:
    void draw();
    void moveWith( const Vec2 &pos);
    void moveTo( const Vec2 &pos);
    Vec2 getPos();
    unsigned int getId();
    Color getColor();
    float getRadius();

    
    Circle(Vec2 cPos, float cRadius, Color cColor);
};

std::ostream& operator << (std::ostream& stream, Circle& obj);