#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <Arduino.h>

class Button {
  
  private:
    byte _pin;
    int _lastState = HIGH;
    unsigned long _lastPressTime = 0;
    unsigned long _lastReleasedTime = 0;
    unsigned long _lastDblPressed;
    bool _singlePressDetected = false;
    bool _longPressReset = true;
    bool _wasShortPressed = false;
    bool _wasDblPressed = false;
    bool _wasLongPressed = false;
    bool _isLongPressing = false;
    void _init();
    bool _debounced(unsigned long time);
    
  public:
    Button(byte pin);
    void update();
    bool getIsPressing();
    bool getWasShortPressed();
    bool getWasDblPressed();
    bool getWasLongPressed();
    bool getIsLongPressing();
};

#endif