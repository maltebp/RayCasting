#pragma once

class FPSCounter{
    double displayCooldown;
    double lastTickTime;
    void printFPS(double fps);
public:
    double tick();
    FPSCounter();
};