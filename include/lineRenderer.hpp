#pragma once

#include<vector>
#include<map>
#include<line.hpp>

class LineRenderer{



private:

    static unsigned int program;
    static bool initialized;

    std::map< float, std::vector<float> > vertices;

    unsigned int vao;
    unsigned int vbo;

public:
    LineRenderer();

    void flush();
    void drawLine(const Line &line);
};


// FPS: 76