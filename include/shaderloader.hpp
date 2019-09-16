#pragma once

#include <string>

unsigned int compileShader(unsigned int type, const std::string &source);

unsigned int createProgram(const std::string &vsSource, const std::string &fsSource);

unsigned int createShader(unsigned int shaderType, const std::string &sourceFile );

int loadShaderSource( std::string filename, std::string &source );