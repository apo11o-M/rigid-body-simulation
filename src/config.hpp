#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Config {
    const int windowWidth = 800;
    const int windowHeight = 450;    
    const int textWidth = 500;
    const int textHeight = 250;
    // The input mode for the keyboard. Value 0 is similar to ones in Unreal Engine 4; Value 1 is 
    // similar to ones in minecraft.
    int inputMode = 0;
};

#endif