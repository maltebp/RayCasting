
#include"raycasting.hpp"

#define GLFW_STATIC
#include <GLFW/glfw3.h>

#include<iostream>
#include<vector>
#include<math.h>

#include"line.hpp"
#include"circle.hpp"
#include"lineRenderer.hpp"

using namespace std;

// ================================


#define NUM_WALLS 10
#define NUM_RAYS 75

#define PI 3.1415926535897

const float FOV =   60*  PI/180;

/* Can't initialize a circle object here, since it
 uses gl objects before it has been initialized */
Circle* cursorCircle = nullptr;
vector<Line*> walls; 
vector<Line*> rays;
vector<Line*> projections;
vector<Line*> boundaries;
LineRenderer lineRenderer;

float viewDirection;

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

        walls.push_back( new Line(startPos, endPos, 5, Color(1,1,1)) );
    }

    boundaries.push_back( new Line( Vec2(0,0), Vec2(screen2D.x,0) ) );
    boundaries.push_back( new Line( Vec2(screen2D.x,0), Vec2(screen2D.x,screen2D.y) ) );
    boundaries.push_back( new Line( Vec2(screen2D.x,screen2D.y), Vec2(0,screen2D.y) ) );
    boundaries.push_back( new Line( Vec2(0,screen2D.y), Vec2(0,0) ) );

    // Initialize Rays and projections
    for(int i=0; i<screen3D.x;i++){
        rays.push_back( new Line( Vec2(0,0), Vec2(0,0), 1, Color(1,1,1) )  );
        projections.push_back( new Line( Vec2(0,0), Vec2(0,0), 1, Color(1,1,1) )  );
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
        int result = getIntersection(pos, angle, *walls[i], intersection);
        if( result ){
            float distance = distanceBetweenPoints(pos, intersection);
            if( minDistance == -1 || minDistance > distance ){
                minDistance = distance;
                bestPoint = intersection;
            }
        }
    }

    for(int i=0; i<4; i++){
        int result = getIntersection(pos, angle, *boundaries[i], intersection);
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
    viewDirection += mouseCoords.x*0.01;
    while(viewDirection > 2*PI){
        viewDirection -= 2*PI;
    }
    while(viewDirection < 0){
        viewDirection += 2*PI;
    }
}



void updateRayCasting(){

    Vec2 pos = cursorCircle->getPos();

    float vel = 0;
    if( moveKeysPressed[FORWARD] ) vel += 4;
    if( moveKeysPressed[BACKWARD]) vel -= 4;

    Vec2 movement(
        cos(viewDirection) * vel,
        sin(viewDirection) * vel
    );

    // Update/Draw cursor
    cursorCircle->moveWith(movement);

    Vec2 newPos = cursorCircle->getPos();

    if( newPos.x < 0 ){
        newPos.x = 0;
    }
    else if( newPos.x > screen2D.x){
        newPos.x = screen2D.x;
    }

    if( newPos.y < 0 ){
        newPos.y = 0;
    }else if(newPos.y > screen2D.y ){
        newPos.y = screen2D.y;
    }

    cursorCircle->moveTo(newPos);
    cursorCircle->draw();



    // Draw Walls
    for( int i=0; i<NUM_WALLS; i++){
        walls[i]->draw();
    }

    
    // Updating and Drawing rays
    float startAngle = viewDirection-FOV/2;
    float angleInc = FOV/screen3D.x;
    float projDist = (screen3D.x/2) / tan(FOV/2);

    for( int i=0; i<screen3D.x; i++ ){

        float angle = startAngle + angleInc*i;

        rays[i]->movePointTo(0, cursorCircle->getPos());
        rays[i]->movePointTo(1, getRayEndPoint( rays[i]->getPos(0), angle )  );
        
        lineRenderer.drawLine(*rays[i]);

        float distance = distanceBetweenPoints(rays[i]->getPos(0), rays[i]->getPos(1)) * cos( viewDirection - angle );

        float x3D = screen2D.x + i;
        float height = (screen2D.y*0.1)/distance * projDist;
        float yAdjust = (screen2D.y-height)/2;
        Color color( 1/(distance*0.01) );

        projections[i]->movePointTo(0, Vec2( x3D, yAdjust ));
        projections[i]->movePointTo(1, Vec2( x3D, screen2D.y - yAdjust));
        projections[i]->setColor(color);

        lineRenderer.drawLine(*projections[i]);
    }


    lineRenderer.flush();
}

void printKeys(){
    cout<<  "Forward: "<<moveKeysPressed[FORWARD]<<
            "Backwards: "<<moveKeysPressed[BACKWARD]<<
            "Left: " << moveKeysPressed[LEFT]<<
            "Right: "<< moveKeysPressed[RIGHT]<<
    endl;;
}
