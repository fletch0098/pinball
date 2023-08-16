#ifndef MY_BUZZER_H
#define MY_BUZZER_H

#include <Arduino.h>
#include "Pitches.h"

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

struct Note { 
    int frequency;
    int duration;
};



class Buzzer {
  
  private:
    byte _pin;
    unsigned long _startMillis;
    int _index = -1;
    int _size = 0;
    void _init();
    struct Note *_notes;

  public:
    // Constructor Method
    Buzzer(byte pin);
    // Update Routine
    void update();

    // Turn on
    void playNote(int frequency, int duration = 0);
    // Turn off
    void off();

    void playMelodySync(Note notes[], int size);
    void playMelody(Note notes[], int size);
    
};

#endif