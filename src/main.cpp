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

using namespace std;

Vec2 screen(1360,960);

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