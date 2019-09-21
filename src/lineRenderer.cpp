
#include <lineRenderer.hpp>


#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.hpp"
#include <iomanip> // Remove this!


// ==============================================================

unsigned int LineRenderer::program = 0;
bool LineRenderer::initialized = false;

// =============================================================

LineRenderer::LineRenderer(){

    // Setup Vertex Array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
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

    std::vector<float> *wVertices;


    for( std::map<float,std::vector<float>>::iterator it = vertices.begin(); it != vertices.end(); it++ ){
        wVertices = &(it->second);

        if( wVertices->size() > 0 ){
            glLineWidth(it->first);
        
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)* wVertices->size(), &(*wVertices)[0], GL_DYNAMIC_DRAW );

            glDrawArrays(GL_LINES, 0, wVertices->size()/6);

            wVertices->clear();
        }   
    }  
}


/** Adds line to buffer */
void LineRenderer::drawLine(const Line &line){

    Vec2 pos1 = line.getPos(0);
    Vec2 pos2 = line.getPos(1);
    Color col = line.getColor();
    float width = line.getWidth();

    std::vector<float> *wVertices;

    try{
        wVertices = &vertices.at(width);
    }catch(std::out_of_range e){
        std::cout<<"Creating new width vertex: "<<std::setprecision(1)<<width<<std::endl;
        wVertices = new std::vector<float>;
        vertices.insert( std::pair<float,std::vector<float>>( width, *wVertices ) );
        
    }
    
    wVertices->push_back( pos1.x );
    wVertices->push_back( pos1.y );
    wVertices->push_back( col.r );
    wVertices->push_back( col.g );
    wVertices->push_back( col.b );
    wVertices->push_back( col.a );
    wVertices->push_back( pos2.x );
    wVertices->push_back( pos2.y );
    wVertices->push_back( col.r );
    wVertices->push_back( col.g );
    wVertices->push_back( col.b );
    wVertices->push_back( col.a );
}