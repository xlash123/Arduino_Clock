#include  <Wire.h>
#include <VariableTimedAction.h>
#include <SPI.h>
#include <SD.h>

#include "LCD.h"
#include "Timer.cpp"
#include "WebServer.cpp"

#include <time.h>

time_t currentTime = 1553116366;

char temp = 'A';

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  LiquidCrystal_I2C lcdRaw(0x27,ROWS,COLS);  // set the LCD address to 0x27 for a 16 chars and 2 line display
  LCD lcd(lcdRaw);
  pinMode(53, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(53, HIGH);
  bool error = false;
  if(!SD.begin(4)){
    LCD::writeString("SD not accessible");
    error = true;
  }
  while(error){
    delay(1);
  }
  WebServer webServer;
  Timer timer(&currentTime);
  delay(1);
  Serial.println("No errors");
  timer.start(1000);
  webServer.start(1000);
}

void loop()
{
  VariableTimedAction::updateActions();
}
