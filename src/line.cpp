#include "line.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.hpp"

// class Line{

//     static bool initialized;
//     static unsigned int program;
//     static unsigned int nextId;

//     unsigned int id;
//     Vec2 point1;
//     Vec2 point2;
//     Color color;
//     float width;


// public:
//     Line(Vec2 p1, Vec2 p2, float w, Color col); 
//     void draw();
//     Vec2 getPos(unsigned int pointIndex);
//     void moveTo(unsigned int pointIndex, Vec2 newPos);
// }; 



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

    if( !initialized ){
        initialized = true;
        program = createProgram("res/vertexshader.shader", "res/fragmentshader.shader");
    }
}



void Line::draw(){
    glLineWidth(width);

    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(currentProgram != program){
        glUseProgram(program);
    }

    float vertices[12] = {
        point1.x, point1.y,
        color.r, color.g, color.b, color.a,
        point2.x, point2.y,
        color.r, color.g, color.b, color.a,
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_DYNAMIC_DRAW );
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_LINES, 0, 2);
    //glDrawElements(GL_LINE, 2, GL_UNSIGNED_INT, nullptr);

    glDeleteBuffers(1, &vbo); 
}



float Line::getWidth(){ return width; }
Color Line::getColor(){ return color; }
Vec2 Line::getPos(unsigned int index){ return index ? point2 : point1; }
unsigned int Line::getId(){return id;}

std::ostream& operator << (std::ostream& stream, Line obj){
    return stream<<"Line{ "<<
        " ID: "<<obj.getId()<<
        ", Point1: "<<obj.getPos(0)<<
        ", Point2: "<<obj.getPos(1)<<
        ", Width: "<<obj.getWidth()<<
        ", Color: "<<obj.getColor()<<
    " }";
}

