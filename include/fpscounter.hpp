#pragma once

class FPSCounter{

    long unsigned int sum;
    long unsigned int count;

    double displayCooldown;
    double lastTickTime;
    void printFPS(double fps);
public:
    double tick();
    FPSCounter();
};