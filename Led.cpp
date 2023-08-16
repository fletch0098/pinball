#include "Led.h"

// Constructor
Led::Led(byte pin) {
  this->_pin = pin;
  this->_state = LOW;
  this->_isLongFlash = false;
  this->_isShortFlash = false;
  this->_isCustom = false;
  this->_isSteady = false;
  this->_isBlink = false;
  _init();
}

// Initialize the LED
void Led::_init() {
  // Setup Pin for output to drive LED
  pinMode(_pin, OUTPUT);
  // Ensure it is off
  off();
}

// Turn LED On
void Led::_on() {
  // Set the state
  _state = HIGH;
  // Write state to pin
  _writeState();
}

// Turn LED On
void Led::_off() {
  // Set the state
  _state = LOW;
  // Write state to pin
  _writeState();
}

// Turn LED On
void Led::on() {
  _isSteady = true;
  _on();
}

// Turn LED Off
void Led::off() {
  _isShortFlash = false;
  _isLongFlash = false;
  _isSteady = false;
  _isBlink = false;
  _isBlinkFast = false;
  _isCustom = false;
  _off();
}

// Write the state to the Pin
void Led::_writeState() {
  digitalWrite(_pin, _state);
}

// Blink with delay
void Led::flashShortSync() {
   _on();
   delay(ONE_HALF_SECOND);
   _off();
}

// Blick without delay
void Led::flashShort() {
  _isShortFlash = true;
  _isLongFlash = false;
  _isSteady = false;
  _isBlink = false;
  _isCustom = false;
  _isBlinkFast = false;
  _on();

   // Store the time it was turned of
   _onMillis = millis();
}

// Blink with delay
void Led::flashLongSync() {
   _on();
   delay(ONE_SECOND);
   _off();
}

// Blick without delay
void Led::flashLong() {
  _isShortFlash = false;
  _isLongFlash = true;
  _isSteady = false;
  _isBlink = false;
  _isCustom = false;
  _isBlinkFast = false;
   _on();

   // Store the time it was turned of
   _onMillis = millis();
}

void Led::flash(int ms) {
  _custom = ms;
   // Store the time it was turned of
   _onMillis = millis();
   _isShortFlash = false;
  _isLongFlash = false;
  _isSteady = false;
  _isBlink = false;
  _isCustom = true;
  _isBlinkFast = false;

  _on();
}

void Led::blink() {
   // Store the time it was turned of
   _onMillis = millis();
   _isShortFlash = false;
  _isLongFlash = false;
  _isSteady = false;
  _isBlink = true;
  _isCustom = false;
  _isBlinkFast = false;

  _on();
}

void Led::blinkFast() {
   // Store the time it was turned of
   _onMillis = millis();
   _isShortFlash = false;
  _isLongFlash = false;
  _isSteady = false;
  _isBlink = false;
  _isBlinkFast = true;
  _isCustom = false;

  _on();
}

// Get the state of the LED as bool
bool Led::isOn() {
  return (_state == HIGH);
}

// Update the LED
void Led::update(){

  // Get current Time
  unsigned long now = millis();

  if(_isShortFlash){
    // If longer than Blink time, turn off
    if (now - _onMillis >= ONE_HALF_SECOND) {
      off();
      _isShortFlash = false;
    }
  } else if(_isLongFlash) {
    // If longer than Blink time, turn off
    if (now - _onMillis >= ONE_SECOND) {
      off();
      _isLongFlash = false;
    }
  } else if(_isBlink) {
    // If longer than Blink time, turn off
    if (now - _onMillis >= ONE_SECOND) {
      _onMillis = now;
      if(isOn() == true){
        _off();
      } else {
        _on();
      }
    }
  } else if(_isBlinkFast) {
    // If longer than BlinkFast time, turn off
    if (now - _onMillis >= ONE_HALF_SECOND) {
      _onMillis = now;
      if(isOn() == true){
        _off();
      } else {
        _on();
      }
    }
  } else if(_isCustom) {
    // If longer than Blink time, turn off
    if (now - _onMillis >= _custom) {
      _off();
      _isCustom = false;
    }
  }

}
