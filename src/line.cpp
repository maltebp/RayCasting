#include "line.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.hpp"

//unsigned int Line::program = 0;
unsigned int Line::nextId = 0;
bool Line::initialized = false;
unsigned int Line::program = 0; 


Line::Line(Vec2 p1, Vec2 p2, float w, Color col){

    id = 0;// nextId++;
    point1 = p1;
    point2 = p2;
    width = w;
    color = col;

    // if( !initialized ){
    //     initialized = true;
    //     program = createProgram("res/vertexshader.shader", "res/fragmentshader.shader");
    // }

    // setupGl();
}

Line::Line(Vec2 p1, Vec2 p2){
    id = 0;
    point1 = p1;
    point2 = p2;
    width = 0;
    color = Color(1,1,1);

    // if( !initialized ){
    //     initialized = true;
    //     program = createProgram("res/vertexshader.shader", "res/fragmentshader.shader");
    // }
    //setupGl();
}





// void Line::draw(){
    
//     int currentProgram;
//     glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
//     if(currentProgram != program){
//         glUseProgram(program);
//     }

//     glLineWidth(width);
//     glBindVertexArray(vao);
//     glBindBuffer( GL_ARRAY_BUFFER, vbo);
    
//     glDrawArrays(GL_LINES, 0, 2);
// }


// void Line::setupGl(){
//     glGenVertexArrays(1, &vao);
//     glBindVertexArray(vao);
    
//     glGenBuffers(1, &vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
//     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float)));
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
    
//     updateVertices();
// }


// void Line::updateVertices(){
//     vertices[0] = point1.x;
//     vertices[1] = point1.y;
//     vertices[2] = color.r;
//     vertices[3] = color.g;
//     vertices[4] = color.b;
//     vertices[5] = color.a;
//     vertices[6] = point2.x;
//     vertices[7] = point2.y;
//     vertices[8] = color.r;
//     vertices[9] = color.g;
//     vertices[10] = color.b;
//     vertices[11] = color.a;

//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_DYNAMIC_DRAW );
// }


float Line::getWidth() const{ return width; }
Color Line::getColor() const{ return color; }
Vec2 Line::getPos(unsigned int index) const{ return index ? point2 : point1; }
unsigned int Line::getId() const{return id;}

void Line::setColor(const Color& col){
    color = col;
}

void Line::movePointTo(unsigned int pointIndex, const Vec2 &pos){
    if(pointIndex == 0){
        point1 = pos;
    }else{
        point2 = pos;
    }
}

std::ostream& operator << (std::ostream& stream, Line obj){
    return stream<<"Line{ "<<
        " ID: "<<obj.getId()<<
        ", Point1: "<<obj.getPos(0)<<
        ", Point2: "<<obj.getPos(1)<<
        ", Width: "<<obj.getWidth()<<
        ", Color: "<<obj.getColor()<<
    " }";
}

