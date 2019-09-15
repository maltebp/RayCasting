#include "shaderloader.hpp"

#define GLEW_STATIC

#include <GL/glew.h>
#include <fstream>
#include <iostream>



unsigned int compileShader(unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type);
    const char* cSource = source.c_str();
    glShaderSource(id, 1, &cSource, nullptr ); 
    glCompileShader(id);
    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if( status == GL_TRUE ){
        std::cout<<"Shader compile successful!"<<std::endl;
    }else{
        int logLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        char* message = (char*) malloc(sizeof(char)*logLength);
        glGetShaderInfoLog(id, logLength, NULL, message); 

        std::cout<<"Compile error with "<<((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader:"<<std::endl;
        std::cout<<message<<std::endl;
        free(message);
        return 0;
    } 
    return id;
}


int loadShaderSource( std::string filename, std::string &source ){
    std::ifstream inFile(filename);
    std::string input(
        (std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>()    
    );
    source += input;
    return 1;
}

unsigned int createShader(unsigned int shaderType, const std::string &sourceFile ){
    std::string sourceText;
    loadShaderSource(sourceFile, sourceText);
    return compileShader(shaderType, sourceText);
}



unsigned int createProgram( const std::string &vsSource, const std::string &fsSource ){
    unsigned int program = glCreateProgram();
    
    unsigned int vertexShader = createShader( GL_VERTEX_SHADER, vsSource);
    glAttachShader(program, vertexShader);
    
    unsigned int fragmentShader = createShader( GL_FRAGMENT_SHADER, fsSource);
    glAttachShader(program, fragmentShader);
    
    if(fragmentShader == 0 || vertexShader == 0){
        glDeleteProgram(program);
    }else{
        glLinkProgram(program);
        glValidateProgram(program);
        return program;
    }
    return 0;    
} 

