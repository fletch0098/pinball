
#include "Led.h"
#include "Button.h"
#include "Buzzer.h"
#include "Bumper.h"
#include <LiquidCrystal_I2C.h>

// Defines
#define BMP_1 8
#define BTN_1 7 
#define BUZZ_1 4
#define LED_1 2

struct Player { 
    String name;
    int points;
};

struct LcdState { 
    bool redraw = false;
    bool displayOn = true;
    int flashCount = 0;
    unsigned long lastFlash;
};

struct Note startMelody[] = { {NOTE_C5, 100}, {NOTE_D5, 100}, {NOTE_E5, 100}, {NOTE_F5, 100}, {NOTE_G5, 100}, {NOTE_A5, 100}, {NOTE_B5, 100}, {NOTE_C6, 500} };
struct Note pointsMelody[] = { {NOTE_E5, 100}, {NOTE_CS5, 100}, {NOTE_A4, 100} };
struct Note coinMelody[] = { {NOTE_B5, 100}, {NOTE_E6, 500} };
struct Note oneUpMelody[] = { {NOTE_E6, 125}, {NOTE_G6, 125}, {NOTE_E7, 125}, {NOTE_C7, 125}, {NOTE_D7, 125}, {NOTE_G7, 125} };
struct Note fireMelody[] = { {NOTE_G4, 35}, {NOTE_G5, 35}, {NOTE_G6, 35} };

const int FlashDelay = 100;
const String Points = "Points X";

// Instantiate Classes
Led led1(LED_1);
Button btn1(BTN_1);
Buzzer buzz1(BUZZ_1);
Bumper bmp1(BMP_1);
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

Player player1 = { "Player 1", 0 };
Player player2 = { "Player 2", 0 };
bool isPlayer1Turn = true;

LcdState lcdState;

// the setup function runs once when you press reset or power the board
void setup() {

  // Setup serial for logging
  Serial.begin(9600);

  logInfo("App: Start");

  buzz1.playMelody(startMelody, ARRAY_LEN(startMelody));

  led1.flash(1200);

  lcdInit();

  lcdDraw(player1.name + " Ball 1", getPlayerPoints(player1.points));

  lcdState.flashCount = 7;
  lcdState.lastFlash = millis();

}

// the loop function runs over and over again forever
void loop() {

  // Update routines
  led1.update();
  btn1.update();
  buzz1.update();
  bmp1.update();
  lcdUpdate();

  if(bmp1.getWasShortHit()){
    logInfo("WasShortHit");
    led1.flashShort();
    buzz1.playMelody(coinMelody, ARRAY_LEN(coinMelody));

    lcdState.flashCount = 4;
    lcdState.lastFlash = millis();

    lcdState.redraw = true;
    if(isPlayer1Turn == true){
      player1.points = player1.points + 10;
    } else {
      player2.points = player2.points + 10;
    }
  }

  if(btn1.getWasShortPressed()){
    logInfo("WasShortPressed");
    led1.flashLong();
    buzz1.playMelody(fireMelody, ARRAY_LEN(fireMelody));

    lcdState.flashCount = 2;
    lcdState.lastFlash = millis();

    lcdState.redraw = true;
    isPlayer1Turn = !isPlayer1Turn;
  }

  if(btn1.getIsLongPressing()){
    logInfo("IsLongPressing");
    led1.flash(1200);
    buzz1.playMelody(startMelody, ARRAY_LEN(startMelody));

    lcdState.flashCount = 7;
    lcdState.lastFlash = millis();

    lcdState.redraw = true;
    if(isPlayer1Turn == true){
      player1.points = 0;
    } else {
      player2.points = 0;
    }
  }

}

// Logging functions for ease
void logInfo(String message){
  // New line
  Serial.println(message);
}

void lcdUpdate() {

  // Now to ensure all time the same
  unsigned long now =  millis();

  if(lcdState.flashCount > 0 && ((now - lcdState.lastFlash) >= FlashDelay)){
    if(lcdState.displayOn == true){
      lcd.noDisplay();
      lcdState.displayOn = false;
    } else {
      lcd.display();
      lcdState.displayOn = true;
    }
    lcdState.lastFlash = now;
    lcdState.flashCount--;
  } else if(lcdState.flashCount <= 0){
    lcd.display();
    lcdState.flashCount = 0;
    lcdState.displayOn = true;
  }

  if(lcdState.redraw == true){
    lcdClear();
    if(isPlayer1Turn == true){
      lcdDraw(player1.name, getPlayerPoints(player1.points));
    } else {
      lcdDraw(player2.name, getPlayerPoints(player2.points));
    }
    lcdState.redraw = false;
  }
}

void lcdInit(){
  // initialize the lcd
  lcd.init();
  lcd.backlight();
}

void lcdClear(){
  lcd.clear();
}

void lcdDraw(String line1, String line2){
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

String getPlayerPoints(int points){
  String x = Points;
  x.replace("X", (String)points);
  return x;
}