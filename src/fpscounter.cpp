#include "fpscounter.hpp"

#include <iostream>
#include <iomanip>
#include <GLFW/glfw3.h>

#define DISPLAY_FREQ 1


void FPSCounter::printFPS(double fps){
    std::cout<<"FPS: "<<std::setprecision(2)<<fps<<std::endl;
}

FPSCounter::FPSCounter(){
    displayCooldown = DISPLAY_FREQ;
    lastTickTime = 0;
}

double FPSCounter::tick(){
    double time = glfwGetTime();
    double dTime = time-lastTickTime;
    displayCooldown -= dTime;
    while( displayCooldown < 0 ){
        printFPS( 1/dTime );
        displayCooldown += DISPLAY_FREQ;
    }

    //printf("Time: %.3f\t LastTime: %.3f\t dTime: %.2f\t CD: %.2f\tFPS: %.2f\n", time, lastTickTime, dTime, displayCooldown, 1/dTime);
    lastTickTime = time;
    return dTime;
}