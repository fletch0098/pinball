#include "Bumper.h"

// Bumper Constants
const int BUMPER_DEBOUNCE_DELAY = 1000;

// Constructor
Bumper::Bumper(byte pin) {
  this->_pin = pin;
  this->_lastState = HIGH;
  _init();
}

// Initialize
void Bumper::_init() {
  pinMode(_pin, INPUT_PULLUP);
  update();
}

// Update and set event flags
void Bumper::update() {

  // Now to ensure all time the same
  unsigned long now =  millis();

  // Reset all event flags 
  _wasShortHit = false;

  // Update the pin status => current state
  int currentState = digitalRead(_pin);

  // Handle change states
  if(_lastState == HIGH && currentState == LOW){
    // Bumper is hit 

  } else if(_lastState == LOW && currentState == HIGH) {
    // Bumper is released

    // Raise event flag
    _wasShortHit = true;
  }

  // Save the currentstate as the last state for next update
  _lastState = currentState;
}

bool Bumper::getWasShortHit() {
  return _wasShortHit;
}

bool Bumper::_debounced(unsigned long time){
  return ((millis() - time) > BUMPER_DEBOUNCE_DELAY);
}