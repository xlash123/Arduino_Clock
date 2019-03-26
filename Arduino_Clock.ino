#include  <Wire.h>
#include <VariableTimedAction.h>
#include <SPI.h>
#include <SD.h>

#include "LCD.h"
#include "Timer.cpp"
#include "WebServer.cpp"

#include <time.h>

LiquidCrystal_I2C lcdRaw(0x27,ROWS,COLS);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LCD lcd(lcdRaw);

time_t currentTime;

Timer timer(&currentTime);
WebServer webServer;

char temp = 'A';

void setup()
{
  Serial.begin(9600);
  currentTime = 1553116366;//Get current time
  bool error = false;
  if(!webServer.server){
    LCD::writeString("No Internet connection");
    error = true;
  }
  if(!SD.begin(4)){
    LCD::writeString("SD not assesssible");
    error = true;
  }
  while(error){
    delay(1);
  }
  timer.start(1000);
  webServer.start(1000);
}

void loop()
{
  VariableTimedAction::updateActions();
}
