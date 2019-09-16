#pragma once

#include "vec2.hpp"

void initializeRayCasting(Vec2 &screenDimensions);
void updateRayCasting( );
void updateMouse( const Vec2 &mouseCoords);
void keyPressed(int keyCode);
void keyReleased(int keyCode);  