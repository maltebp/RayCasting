
#include"raycasting.hpp"

#include<iostream>
#include<vector>

#include"line.hpp"
#include"circle.hpp"

using namespace std;

// ================================

vector<Line> walls;
Circle cursorCircle( Vec2(0,0), 20, Color(1,1,1));


// ================================

void updateRayCasting(Vec2 *screenDimensions, Vec2 *mouseCoords){


    // Drawing cursor
    cursorCircle.moveTo(*mouseCoords);
    cursorCircle.draw();

}