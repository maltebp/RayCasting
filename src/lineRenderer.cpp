
#include <lineRenderer.hpp>


#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.hpp"


// ==============================================================

unsigned int LineRenderer::program = 0;
bool LineRenderer::initialized = false;

// =============================================================

LineRenderer::LineRenderer(){

    // Setup Vertex Array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Telling it should advance each attribute instance     
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &vbo);

    if( !initialized ){
        initialized = true;
        program = createProgram("res/vertexshader.shader", "res/fragmentshader.shader");
    }
}



void LineRenderer::flush(){

    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(currentProgram != program){
        glUseProgram(program);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW );

    glDrawArrays(GL_LINES, 0, 2);
}


/** Adds line to buffer */
void LineRenderer::drawLine(const Line &line){

    Vec2 pos1 = line.getPos(0);
    Vec2 pos2 = line.getPos(1);
    Color col = line.getColor();

    vertices.push_back( pos1.x );
    vertices.push_back( pos1.y );
    vertices.push_back( col.r );
    vertices.push_back( col.g );
    vertices.push_back( col.b );
    vertices.push_back( col.a );
    
    vertices.push_back( pos2.x );
    vertices.push_back( pos2.y );
    vertices.push_back( col.r );
    vertices.push_back( col.g );
    vertices.push_back( col.b );
    vertices.push_back( col.a );

}