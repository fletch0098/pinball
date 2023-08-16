#ifndef MY_BUMPER_H
#define MY_BUMPER_H

#include <Arduino.h>

class Bumper {
  
  private:
    byte _pin;
    int _lastState = HIGH;
    bool _wasShortHit = false;
    void _init();
    bool _debounced(unsigned long time);
    
  public:
    Bumper(byte pin);
    void update();
    bool getWasShortHit();
};

#endif