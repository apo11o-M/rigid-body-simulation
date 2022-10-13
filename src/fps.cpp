#include <iostream>
#include <GLFW/glfw3.h>

#include "fps.hpp"

void Fps::update() {
    double currTime = glfwGetTime();
    mFrame++;
    // update the frame counter every 0.5 seconds.
    if (currTime - prevTime >= 0.5) {
        mFps = mFrame * 2;
        mFrame = 0;
        prevTime = currTime;
    }    
}

short int Fps::getFps() {
    return mFps;
}