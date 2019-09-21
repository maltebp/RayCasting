#pragma once

#include<vector>
#include<line.hpp>

class LineRenderer{


private:

    std::vector<float> vertices;

    unsigned int vao;
    unsigned int vbo;

public:
    LineRenderer();
    void flush();
    void drawLine(const Line &line);
};


// FPS: 76