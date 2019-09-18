#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "fpscounter.hpp"
#include <time.h>
#include "vec2.hpp"
#include "raycasting.hpp"
//#include "raycasting.hpp"

using namespace std;




Vec2 screen(1360,960);
//Circle *circle = nullptr;
//vector<Line> lines;
//vector<Circle> circles;

void glfwErrorCallback(int errCode, const char* errStr){
    cout<<"GLFW Error: "<<"("<<errCode<<") "<<errStr<<endl;
}


static void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mod  ){
    if( action == GLFW_PRESS){
        keyPressed(key);
    }
    if( action == GLFW_RELEASE){
        keyReleased(key);
    }
}



static void cursorPosCallback(GLFWwindow *window, double x, double y){
    double dx = x-screen.x/2; 
    double dy = y-screen.y/2;
    glfwSetCursorPos(window, screen.x/2, screen.y/2);
    updateMouse( Vec2(dx,dy) );
}




int main(void)
{
    cout<<"Starting..."<<endl;

    /* Initialize the library */
    if (!glfwInit()){
        cout<<"GLFW init  error"<<endl;
        return -1; 
    }

    GLFWwindow* window;

    glfwSetErrorCallback(glfwErrorCallback);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_SAMPLES, 16);
    window = glfwCreateWindow(1360, 960, "Hello World", NULL, NULL);
    if (!window)
    {
        cout<<"Window creation error"<<endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if(glewInit() != GLEW_OK){
        std::cout<<"Glew not ok"<<std::endl;
    }

    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallBack);

    srand(time(NULL));

    // Line line( Vec2(100.,100.), Vec2(400.,100.), 5., Color(1.,1.,1.) );
    // cout<<"Created line: "<<endl<<line<<endl;

    // /*
    // for(int i=0; i<100; i++){
    //     Circle c( Vec2(rand()%1360, rand()%960), 12, Color(1,1,1));
    //     circles.push_back(c);
    // }*/

    // for(int i=0; i<20;i++){
    //     Vec2 startPos = Vec2(rand()%960,rand()%560);
    //     Vec2 endPos = Vec2( startPos.x + rand()%400+50, startPos.y + rand()%400+50 );

    //     lines.push_back( Line( startPos, endPos, 5, Color(1,1,1) )  );
    // }

    // Circle circleObj( Vec2(30,30), 15, Color(0.5,1.0,0.5) );
    // circle = &circleObj;
    // //cout<<"Created circle:"<<endl<<circle<<endl;

    FPSCounter fpsCounter;

    initializeRayCasting(screen);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    { 
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_MULTISAMPLE);

        updateRayCasting();
        fpsCounter.tick();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    cout<<"Exitted loop"<<endl;

    glfwTerminate();

    cout<<endl<<"Finished"<<endl;
    return 0;
}