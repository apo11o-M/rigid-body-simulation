// Calculates and updates the fps of the current window
#ifndef FPS_HPP
#define FPS_HPP

class Fps {
    public:
        Fps() : mFrame(0), mFps(0), prevTime(0) {};
    private:
        unsigned int mFrame;
        unsigned short int mFps;
        double prevTime;
    public:
        // updates the frame count
        void update();
        // returns the frame counter in the past 0.5 seconds
        short int getFps();              
};

#endif