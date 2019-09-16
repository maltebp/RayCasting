
#include"raycasting.hpp"

#include<iostream>
#include<vector>
#include<math.h>

#include"line.hpp"
#include"circle.hpp"

using namespace std;

// ================================


#define NUM_WALLS 10
#define NUM_RAYS 500

#define PI 3.1415926535897


/* Can't initialize a circle object here, since it
 uses gl objects before it has been initialized */
Circle *cursorCircle = nullptr;
vector<Line> walls; 
vector<Line> rays;
vector<Line> boundaries;

// ================================


void initializeRayCasting(){

    cursorCircle = new Circle( Vec2(0,0), 20, Color(1,1,1));
    
    // Initialize Walls
    for(int i=0; i<NUM_WALLS;i++){
        Vec2 startPos = Vec2(rand()%960,rand()%560);
        Vec2 endPos = Vec2( startPos.x + rand()%400+50, startPos.y + rand()%400+50 );
        walls.push_back( Line( startPos, endPos, 5, Color(1,1,1) )  );
    }

    boundaries.push_back( Line( Vec2(0,0), Vec2(1360,0) ) );
    boundaries.push_back( Line( Vec2(1360,0), Vec2(1360,960) ) );
    boundaries.push_back( Line( Vec2(1360,960), Vec2(0,960) ) );
    boundaries.push_back( Line( Vec2(0,960), Vec2(0,0) ) );

    // Initialize Rays
    for(int i=0; i<NUM_RAYS;i++){
        rays.push_back( Line( Vec2(0,0), Vec2(0,0), 1, Color(1,1,1) )  );
    }
   
}

static float distanceBetweenPoints(Vec2 &point1, Vec2 &point2){
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


void updateRayCasting(Vec2 *screenDimensions, Vec2 *mouseCoords){

    
    // Updating and Drawing rays
    float rayAngle = 2 * PI /  NUM_RAYS;

    for( int i=0; i<NUM_RAYS; i++ ){
        rays[i].movePointTo(0, *mouseCoords);
        rays[i].movePointTo(1, getRayEndPoint( rays[i].getPos(0), i*rayAngle )  );
        rays[i].draw();
    }

    // Draw Walls
    for( int i=0; i<NUM_WALLS; i++){
        walls[i].draw();
    }
    

    // Update/Draw cursor
    cursorCircle->moveTo(*mouseCoords);
    cursorCircle->draw();


}