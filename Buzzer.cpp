#include "Buzzer.h"

// Constructor
Buzzer::Buzzer(byte pin) {
  this->_pin = pin;
}

// Initialize the Buzzer
void Buzzer::_init() {
  // Ensure it is off
  noTone(_pin);
}

// Turn Off
void Buzzer::off() {
  // Turn off tone
  noTone(_pin);
}

// Play Tone
void Buzzer::playNote(int frequency, int duration = 0) {

  off();

  if(duration == 0){
    // Play until off
    tone(_pin, frequency);
  } else {
    // Play for duration built in async
    tone(_pin, frequency, duration);
  }
  
}

void Buzzer::playMelodySync(struct Note notes[], int size) {

  // Ensure its off and defaults
  off();
  _index = -1;
  _size = 0;

  // Loop
  for (int i = 0; i < size; i++) {

    // Get the current itterations note
    int frequency = notes[i].frequency;
    int duration = notes[i].duration;

    if(frequency != 0){
      // Play note
      playNote(frequency, duration);
      // Pause for duration
      delay(duration);
    } else {
      delay(duration);
    }
  }
}

void Buzzer::playMelody(struct Note notes[], int size){

  // Array length
  _size = size;
  _startMillis = millis();
  _index = 0;
  _notes = notes;
  off();

  // First note
  int frequency = notes[_index].frequency;
  int duration = notes[_index].duration;

  playNote(frequency, duration);

}

// Update Routine
void Buzzer::update(){

  // Check if Playing
  if(_index >= 0 && _index < _size){
    // Playing Melody

    // Get current Time
    unsigned long now = millis();

    int duration = _notes[_index].duration;

    if(now - _startMillis >= duration){
      // Time to change

      int nextFrequency = 0;
      _index++;

      // index of next note to play
      if(_index < _size){
        nextFrequency = _notes[_index].frequency;
        duration = _notes[_index].duration;
      }

       // Prepare for next
      _startMillis = now;

      if(nextFrequency == 0){
        off();
      } else {
        playNote(nextFrequency, duration);
      }
    }
  }
  

}