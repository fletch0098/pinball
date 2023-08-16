#include "Button.h"

// Button Constants
const int LONG_PRESS_TIME = 1500;
const int DBL_PRESS_TIME = 500;
const int DEBOUNCE_DELAY = 1000;

// Constructor
Button::Button(byte pin) {
  this->_pin = pin;
  this->_lastState = HIGH;
  this->_lastDblPressed = millis();
  _init();
}

// Initialize
void Button::_init() {
  pinMode(_pin, INPUT_PULLUP);
  update();
}

// Update and set event flags
void Button::update() {

  // Now to ensure all time the same
  unsigned long now =  millis();

  // Reset all event flags 
  _wasShortPressed = false;
  _wasLongPressed = false;
  _wasDblPressed = false;
  _isLongPressing = false;

  // Update the pin status => current state
  int currentState = digitalRead(_pin);

  // Handle button change states
  if(_lastState == HIGH && currentState == LOW){
    // Button is pressed 

    // Pressed time for Long press and dbl press
    unsigned long currentPressedTime = now;    
  
    // Update the last pressed time
    _lastPressTime = currentPressedTime;

    // ? 
    _singlePressDetected = false; 

  } else if(_lastState == LOW && currentState == HIGH) {
    // button is released

    // Released time for longPress
    unsigned long currentReleasedTime = now;

    // Check which type or press/release it is
    if (currentReleasedTime - _lastPressTime >= LONG_PRESS_TIME){
      // Was Long pressed
      _wasLongPressed = true;

      // ?
      _singlePressDetected = false;
    } else {
      // Was Short pressed and debounced

      // Check if dbl pressed
      if (_lastReleasedTime != 0 && currentReleasedTime - _lastReleasedTime < DBL_PRESS_TIME && _debounced(_lastDblPressed)) {     
        // Dbl Pressed 
        _wasDblPressed = true;
        _singlePressDetected = false;
        _lastReleasedTime = 0;
        _lastDblPressed = now;
      } else {   
        // Could be single pressed
        _singlePressDetected = true;
      }
    }

    // Update last released state
    _lastReleasedTime = currentReleasedTime;

  }

  // Checck for state change
  if(currentState != _lastState){
    // State changed, longPressing avaible again
    _longPressReset = true;
  }

  // Check if not dbl press, set single //check if 500ms have passed with no second button press
  if (_singlePressDetected == true && _wasDblPressed == false && _wasLongPressed == false && _lastReleasedTime != 0 && (now - _lastReleasedTime >= DBL_PRESS_TIME)  && _debounced(_lastDblPressed)) {
    // Single press
    _wasShortPressed = true;
    _singlePressDetected = false;
  }

  // Check for long presssing   
  if(currentState == LOW) {
    // Pressing
    long pressDuration = now - _lastPressTime;

    // Check if pressDuration is long, and Reset flag true 
    if( pressDuration > LONG_PRESS_TIME && _longPressReset == true ) {
      // Long Pressing
        _isLongPressing = true;

        // Reset flag flase until state changed again
        _longPressReset = false;
    }
  }

  // Save the currentstate as the last state for next update
  _lastState = currentState;
}

bool Button::getIsPressing() {
  return (_lastState == LOW);
}

bool Button::getWasShortPressed() {
  return _wasShortPressed;
}

bool Button::getWasDblPressed(){
  return _wasDblPressed;
}

bool Button::getWasLongPressed() {
  return _wasLongPressed;
}

bool Button::getIsLongPressing() {
  return _isLongPressing;
}

bool Button::_debounced(unsigned long time){
  return ((millis() - time) > DEBOUNCE_DELAY);
}