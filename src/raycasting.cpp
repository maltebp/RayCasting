
#include"raycasting.hpp"

#define GLFW_STATIC
#include <GLFW/glfw3.h>

#include<iostream>
#include<vector>
#include<math.h>

#include"line.hpp"
#include"circle.hpp"

using namespace std;

// ================================


#define NUM_WALLS 10
#define NUM_RAYS 75

#define PI 3.1415926535897

const float FOV = 60  *  PI/180;

/* Can't initialize a circle object here, since it
 uses gl objects before it has been initialized */
Circle *cursorCircle = nullptr;
vector<Line> walls; 
vector<Line> rays;
vector<Line> boundaries;

Vec2 mousePos;
Vec2 screen2D;
Vec2 screen3D;


// ================================

typedef enum {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} MoveKey;

bool moveKeysPressed[4] = {false};


void initializeRayCasting(Vec2 &screenDimensions){

    screen2D = Vec2( screenDimensions.x/2, screenDimensions.y);
    screen3D = Vec2( screenDimensions.x/2, screenDimensions.y);

    cursorCircle = new Circle( Vec2(0,0), 20, Color(1,1,1));
    
    // Initialize Walls
    for(int i=0; i<NUM_WALLS;i++){
        Vec2 startPos = Vec2(rand()% (int) screen2D.x,rand()% (int) screen2D.y);
        Vec2 endPos = Vec2( (int) (startPos.x + rand()) % (int) screen2D.x, (int) (startPos.y + rand()) % (int) screen2D.y );
        walls.push_back( Line( startPos, endPos, 5, Color(1,1,1) )  );
    }

    boundaries.push_back( Line( Vec2(0,0), Vec2(screen2D.x,0) ) );
    boundaries.push_back( Line( Vec2(screen2D.x,0), Vec2(screen2D.x,screen2D.y) ) );
    boundaries.push_back( Line( Vec2(screen2D.x,screen2D.y), Vec2(0,screen2D.y) ) );
    boundaries.push_back( Line( Vec2(0,screen2D.y), Vec2(0,0) ) );

    // Initialize Rays
    for(int i=0; i<NUM_RAYS;i++){
        rays.push_back( Line( Vec2(0,0), Vec2(0,0), 1, Color(1,1,1) )  );
    }
}

static float distanceBetweenPoints(const Vec2 &point1, const Vec2 &point2){
    return sqrt( pow(point2.x-point1.x, 2) + pow(point2.y-point1.y, 2) );
}


static int getIntersection( Vec2 &rayPos, float rayAngle, Line &line, Vec2 &intersection){
    float x1 = line.getPos(0).x;
    float y1 = line.getPos(0).y;
    float x2 = line.getPos(1).x;
    float y2 = line.getPos(1).y;


    float x3 = rayPos.x;
    float y3 = rayPos.y;
    float x4 = rayPos.x + cos(rayAngle) * 1;
    float y4 = rayPos.y + sin(rayAngle) * 1;

    float denominator = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    if( denominator == 0 ) return 0;

    float t = ((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4))/denominator;
    float u = -((x1-x2)*(y1-y3)-(y1-y2)*(x1-x3))/denominator; 

    if( t > 0 && t < 1 && u > 0 ){
        intersection.x = x1 + t*(x2-x1);
        intersection.y = y1 + t*(y2-y1);
        return 1;
    }else{
        return 0;
    }

}

static Vec2 getRayEndPoint(Vec2 pos, float angle){

    Vec2 bestPoint;
    Vec2 intersection;
    float minDistance = -1;

    for(int i=0; i<NUM_WALLS; i++){
        int result = getIntersection(pos, angle, walls[i], intersection);
        if( result ){
            float distance = distanceBetweenPoints(pos, intersection);
            if( minDistance == -1 || minDistance > distance ){
                minDistance = distance;
                bestPoint = intersection;
            }
        }
    }

    for(int i=0; i<4; i++){
        int result = getIntersection(pos, angle, boundaries[i], intersection);
        if( result ){
            float distance = distanceBetweenPoints(pos, intersection);
            if( minDistance == -1 || minDistance > distance ){
                minDistance = distance;
                bestPoint = intersection;
            }
        }
    }

    return bestPoint;
}


void keyPressed(int keyCode){
    switch(keyCode){
        case GLFW_KEY_W:
            moveKeysPressed[FORWARD] = true;
            break;
        case GLFW_KEY_S:
            moveKeysPressed[BACKWARD] = true;
            break;
        case GLFW_KEY_A:
            moveKeysPressed[LEFT] = true;
            break;
        case GLFW_KEY_D:
            moveKeysPressed[RIGHT] = true;;
            break;
    }
}

void keyReleased(int keyCode){
    switch(keyCode){
        case GLFW_KEY_W:
            moveKeysPressed[FORWARD] = false;
            break;
        case GLFW_KEY_S:
            moveKeysPressed[BACKWARD] = false;
            break;
        case GLFW_KEY_A:
            moveKeysPressed[LEFT] = false;
            break;
        case GLFW_KEY_D:
            moveKeysPressed[RIGHT] = false;
            break;
    }
}

void updateMouse(const Vec2 &mouseCoords){
    mousePos.x = mouseCoords.x > screen2D.x ? screen2D.x : mouseCoords.x;
    mousePos.y = mouseCoords.y > screen2D.y ? screen2D.y : mouseCoords.y;
}



void updateRayCasting(){

    Vec2 pos = cursorCircle->getPos();
    float direction = atan2( mousePos.y- pos.y, mousePos.x - pos.x);

    float vel = 0;
    if( moveKeysPressed[FORWARD] ) vel += 4;
    if( moveKeysPressed[BACKWARD]) vel -= 4;

    Vec2 movement(
        cos(direction) * vel,
        sin(direction) * vel
    );

    // Update/Draw cursor
    cursorCircle->moveWith(movement);
    cursorCircle->draw();


    // // Updating and Drawing rays
    // float startAngle = direction-FOV/2;
    // float angleStep = FOV/NUM_RAYS;
    // float rayWidth = screen3D.x / NUM_RAYS;

    // for( int i=0; i<NUM_RAYS; i++ ){
    //     rays[i].movePointTo(0, cursorCircle->getPos());
    //     rays[i].movePointTo(1, getRayEndPoint( rays[i].getPos(0), startAngle + i*angleStep )  );
    //     rays[i].draw();

    //     float distance = distanceBetweenPoints(rays[i].getPos(0), rays[i].getPos(1));
    //     float x3D = screen2D.x + rayWidth * i;
    //     float yAdjust = screen3D.y * 0.1 + distance*0.5;
    //     if( yAdjust > screen3D.y/2) yAdjust = screen3D.y/2; 
    //     float color = 1/(distance*0.01);

    //     Line projection( 
    //         Vec2( x3D, yAdjust ),
    //         Vec2( x3D, screen2D.y - yAdjust),
    //         rayWidth,
    //         Color(color,color,color)
    //         );

    //     projection.draw();
    // }

    
    // Updating and Drawing rays
    float startAngle = direction-FOV/2;
    float angleStep = FOV/NUM_RAYS;
    float rayWidth = screen3D.x / NUM_RAYS;

    for( int i=0; i<NUM_RAYS; i++ ){
        float angle = atan( (i-screen3D.x/2) / 50);


        rays[i].movePointTo(0, cursorCircle->getPos());
        rays[i].movePointTo(1, getRayEndPoint( rays[i].getPos(0), startAngle + angle )  );
        rays[i].draw();

        float distance = distanceBetweenPoints(rays[i].getPos(0), rays[i].getPos(1));
        float x3D = screen2D.x + rayWidth * i;
        float yAdjust = screen3D.y * 0.1 + distance*0.5;
        if( yAdjust > screen3D.y/2) yAdjust = screen3D.y/2; 
        float color = 1/(distance*0.01);

        Line projection( 
            Vec2( x3D, yAdjust ),
            Vec2( x3D, screen2D.y - yAdjust),
            rayWidth,
            Color(color,color,color)
            );

        projection.draw();
    }



    // Draw Walls
    for( int i=0; i<NUM_WALLS; i++){
        walls[i].draw();
    }

}



void printKeys(){
    cout<<  "Forward: "<<moveKeysPressed[FORWARD]<<
            "Backwards: "<<moveKeysPressed[BACKWARD]<<
            "Left: " << moveKeysPressed[LEFT]<<
            "Right: "<< moveKeysPressed[RIGHT]<<
    endl;;
}
