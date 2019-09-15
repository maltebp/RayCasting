#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "fpscounter.hpp"
#include "circle.hpp"
#include <time.h>

using namespace std;


Vec2 mousePos;
Circle *circle = nullptr;
vector<Circle> circles;

void glfwErrorCallback(int errCode, const char* errStr){
    cout<<"GLFW Error: "<<"("<<errCode<<") "<<errStr<<endl;
}



static void cursorPosCallback(GLFWwindow *window, double x, double y){
    mousePos.x = (float) x;
    mousePos.y = (float) y;
    cout<<fixed<<"Mouse Pos: "<<mousePos<<endl;
    circle->moveTo( mousePos );
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

    cout<<"Starting..."<<endl<<endl;

    srand(time(NULL));

    for(int i=0; i<100; i++){
        Circle c( Vec2(rand()%1360, rand()%960), 12, Color(1,1,1));
        circles.push_back(c);
    }

    Circle circleObj( Vec2(30,30), 15, Color(0.5,1.0,0.5) );
    circle = &circleObj;
    cout<<"Created circle:"<<endl<<circle<<endl;

    FPSCounter fpsCounter;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    { 
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_MULTISAMPLE);




        circle->draw();

        for(int i=0; i<circles.size(); i++){
            Circle *c = &circles.at(i);
            Vec2 pos = c->getPos();
            pos.x += 10;
            if( pos.x > 1360) pos.x = 0;
            c->moveTo(pos);
            c->draw();
        }
        // for(int i=0; i<20; i++){
        //     circles[i]->draw();
        // }
        //cout<<*circle<<endl;

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