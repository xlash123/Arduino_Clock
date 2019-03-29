#include  <Wire.h>
#include <VariableTimedAction.h>
#include <SPI.h>
#include <SD.h>
//#include <TMRpcm.h>

#include "LCD.h"
#include "Timer.cpp"
#include "WebServer.cpp"

#include <time.h>

time_t currentTime = 1553116366;
//TMRpcm audio;

char temp = 'A';

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  LiquidCrystal_I2C lcdRaw(0x27,ROWS,COLS);  // set the LCD address to 0x27 for a 16 chars and 2 line display
  LCD lcd(lcdRaw);
  LCD::writeString("Initializing...");
  pinMode(53, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(53, HIGH);
  bool error = false;
  if(!SD.begin(4)){
    LCD::writeString("Error:\nSD not accessible");
    error = true;
  }
  while(error){
    delay(1);
  }
  WebServer webServer;
  Timer timer(&currentTime);
  Serial.println("No errors");
  LCD::clearRow(0);
  LCD::clearRow(1);
  timer.start(1000);
  webServer.start(1000);
//  audio.speakerPin = 11;
//  audio.setVolume(5);
//  audio.play("test.wav");
}

void loop()
{
  VariableTimedAction::updateActions();
}
