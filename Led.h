#ifndef MY_LED_H
#define MY_LED_H

#include <Arduino.h>
#include "Constants.h"

class Led {
  
  private:
    byte _pin;
    byte _state;
    bool _isShortFlash;
    bool _isLongFlash;
    bool _isSteady;
    bool _isBlink;
    bool _isBlinkFast;
    bool _isCustom;
    int _custom;
    unsigned long _onMillis;
    void _init();
    void _writeState();
    void _on();
    void _off();

  public:
    // Constructor Method
    Led(byte pin);
    // Update LED Routine
    void update();
    // Get the state of the LED
    bool isOn();
    // Turn LED on
    void on();
    // Turn LED off
    void off();
    
    void flashShort();
    void flashShortSync();
    void flashLong();
    void flashLongSync();
    void flash(int ms);
    void blink();
    void blinkFast();
    
};

#endif