#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "pitches.h"

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;  // 500 miliseconds

// Button pins
const int BTN_POINTS = 8;
const int BTN_PLAYER = 7;

const int LED_POINTS = 2;

// String Consts
const String PLAYER_1 = "Player 1";
const String PLAYER_2 = "Player 2";
const String POINTS = " Points";

const int LONG_PRESS_TIME = 2000; // 2000 milliseconds

// Game state
int pointsP1 = 0;
int pointsP2 = 0;
bool isP1 = true;
bool isPlayerChange = false;

int btnPointsLastState = HIGH;
unsigned long btnPointsPressedTime  = 0;
unsigned long btnPointsReleasedTime = 0;

int btnPlayerLastState = HIGH;
unsigned long btnPlayerPressedTime  = 0;
unsigned long btnPlayerReleasedTime = 0;
bool btnPlayerIsPressing = false;
bool btnPlayerIsLongDetected = false;


void setup()
{
  // Set up logging
  Serial.begin(9600);
  logInfo("Setting up the Pinball app");
  
  // initialize the lcd
  lcd.init();
  lcd.backlight();
    
  // Draw the LCD screen
  draw();

  // Define pins
  pinMode(BTN_POINTS, INPUT_PULLUP);  
  pinMode(BTN_PLAYER, INPUT_PULLUP); 
  pinMode(LED_POINTS, OUTPUT);

  playStartSound();
}

void loop()
{

  // Listen and handle btnPoints events
  btnPointsListener();

  // Listen and handle btnPlayer events
  btnPlayerListner();

  // set the LED:
  draw();

}


void draw(){
  // Clear the screen if player change
  if(isPlayerChange == true){
    lcd.clear();
    isPlayerChange = false;
  }

  // Set the cursor
  lcd.setCursor(0,0);

  // Draw player 1 or 2
  if(isP1){
    // Print
    lcd.print(PLAYER_1);
    lcd.setCursor(0,1);
    lcd.print(pointsP1);
    lcd.print(POINTS);
  } else {
    // Print
    lcd.print(PLAYER_2);
    lcd.setCursor(0,1);
    lcd.print(pointsP2);
    lcd.print(POINTS);
  }
}

void logInfo(String message){
  Serial.print(message);
  Serial.println();
}

void btnPointsListener(){
  // read the state of the switch into a local variable:
  int btnPointsCurrentState = digitalRead(BTN_POINTS);

  if(btnPointsLastState == HIGH && btnPointsCurrentState == LOW){
    // button is pressed   
    logInfo("Button Points Pressed");
  } else if(btnPointsLastState == LOW && btnPointsCurrentState == HIGH) { 
    // button is released
    logInfo("Button Points Released");

    // Add Points
    playPointsSound();
    // score();
    // led();
    if(isP1 == true){
        pointsP1 = pointsP1 + 10;
    } else {
      pointsP2 = pointsP2 + 10;
    }
  }

  // save the the last state
  btnPointsLastState = btnPointsCurrentState;
}

void btnPlayerListner(){
    // read the state of the switch into a local variable:
  int btnPlayerCurrentState = digitalRead(BTN_PLAYER);

  if(btnPlayerLastState == HIGH && btnPlayerCurrentState == LOW){
    // button is pressed        
    btnPlayerPressedTime = millis();
    btnPlayerIsPressing = true;
    btnPlayerIsLongDetected = false;
    logInfo("Button Players Pressed");
  } else if(btnPlayerLastState == LOW && btnPlayerCurrentState == HIGH) {
    // button is released
    btnPlayerReleasedTime = millis();
    btnPlayerIsPressing = false;

    // If not long press change player
    if(btnPlayerIsLongDetected != true){
      isP1 = !isP1;
      // Clear LCD
      isPlayerChange = true;
    }
  
    logInfo("Button Player Released");
  }

  // Check for long press    
  if(btnPlayerIsPressing == true && btnPlayerIsLongDetected == false) {
    long pressDuration2 = millis() - btnPlayerPressedTime;

    if( pressDuration2 > LONG_PRESS_TIME ) {
      logInfo("Button Player Long Pressed");
      btnPlayerIsLongDetected = true;
      isPlayerChange = true;
      playStartSound();
      if(isP1 == true){
        pointsP1 = 0;
      }else {
        pointsP2 = 0;
      }
    }
  }
  
  // save the the last state
  btnPlayerLastState = btnPlayerCurrentState;
}

void flashLcd() {
  lcd.noBacklight();
  delay(100);
  lcd.backlight();
}

void flashDisplay() {
  lcd.noDisplay();
  delay(200);
  lcd.display();
}

void score() {
  lcd.clear();
  // Set the cursor
  lcd.setCursor(0,0);

    // Print
    lcd.print("**** SCORE ****");
    lcd.setCursor(0,1);
    lcd.print("***** +10 *****");
    isPlayerChange = true;
    delay(300);
}

void led() {
    digitalWrite(LED_POINTS, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_POINTS, LOW);   // turn the LED off by making the voltage LOW
}

void playStartSound(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // pin4 output the voice, every scale is 0.5 sencond
    tone(4, melody[thisNote], duration);
     
    // Output the voice after several minutes
    delay(100);
  }
}

void playPointsSound(){
  // play notes 660Hz, 550Hz and 440Hz
  tone(4, 660);
  delay(100);
  tone(4, 550);
  delay(100);
  tone(4, 440);
  delay(100);
  noTone(4);
}

