
#include "circle.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.hpp"


unsigned int Circle::program = 0;
bool Circle::initialized = false;
unsigned int Circle::nextId = 0;

unsigned int Circle::getId(){ return id; }
float Circle::getRadius(){ return radius; }
Color Circle::getColor(){ return color; }
Vec2 Circle::getPos(){ return pos; }

Circle::Circle(Vec2 cPos, float cRadius, Color cColor){
    color = cColor;
    pos = cPos;
    radius = cRadius;
    id = nextId++;
    updateVerticesPos();
    updateVerticesCol();
    
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

    

    if( !initialized ){
        initialized = true;
        program = createProgram("res/vertexshader.shader", "res/circle_fragShader.glsl");
    } 
}



void Circle::updateVerticesPos(){
    vertices[0] = pos.x-radius;
    vertices[1] = pos.y+radius;

    vertices[6] = pos.x+radius;
    vertices[7] = pos.y+radius;

    vertices[12] = pos.x+radius;
    vertices[13] = pos.y-radius;

    vertices[18] = pos.x-radius;
    vertices[19] = pos.y-radius;
}

void Circle::updateVerticesCol(){
    for(int i=2; i<24; i += 6){
        vertices[i] = color.r;
        vertices[i+1] = color.g;
        vertices[i+2] = color.b;
        vertices[i+3] = color.a;
    }
}


void Circle::moveTo(const Vec2 &newPos){
    pos.x = newPos.x;
    pos.y = newPos.y;
    updateVerticesPos();
}


void Circle::draw(){

    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(currentProgram != program){
        glUseProgram(program);
    }

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4*6*sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (0*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);


    unsigned int loc = glGetUniformLocation(program, "center");
    if( loc != -1){
        glUniform2f(loc, pos.x, pos.y);
    }else{
        std::cout<<"Couldn't find uniform 'center' variable!"<<std::endl;
    }
    
    
    loc = glGetUniformLocation(program, "radius");
    if( loc != -1){
        glUniform1f(loc, radius);
    }else{
        std::cout<<"Couldn't find uniform 'radius' variable!"<<std::endl;
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Doing this removes massive lag
    glDeleteBuffers(1, &buffer);
    glDeleteBuffers(1, &ibo);
}



std::ostream& operator << (std::ostream& stream, Circle &obj){
    return stream <<
         "Circle: {"<<
         " ID: "<<obj.getId()<<
         ", Pos: "<<obj.getPos()<<
         ", Radius: "<<obj.getRadius()<<
         ", Color: "<<obj.getColor()<<
         "}";
}

