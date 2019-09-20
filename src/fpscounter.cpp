#include "fpscounter.hpp"

#include <iostream>
#include <iomanip>
#include <GLFW/glfw3.h>

#define DISPLAY_FREQ 1


FPSCounter::FPSCounter(){
    displayCooldown = DISPLAY_FREQ;
    lastTickTime = 0;
    sum = 0; 
    count = 0;
}

double FPSCounter::tick(){
    double time = glfwGetTime();
    double dTime = time-lastTickTime;
    displayCooldown -= dTime;

    while( displayCooldown < 0 ){

        double fps = 1/dTime;

        sum += fps;
        count++;

        std::cout<<std::fixed<<"FPS: "<<std::setprecision(2)<<fps<<" (Avg: "<<sum/(double) count<<")"<<std::endl;
        displayCooldown += DISPLAY_FREQ;
    }

    lastTickTime = time;
    return dTime;
}