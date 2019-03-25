#include  <Wire.h>
#include <VariableTimedAction.h>
#include <Ethernet.h>
#include <SPI.h>

#include "Timer.cpp"
#include "WebServer.cpp"
#include "LcdUtils.h"

#include <time.h>

LiquidCrystal_I2C lcd(0x27,ROWS,COLS);  // set the LCD address to 0x27 for a 16 chars and 2 line display

time_t currentTime;

Timer timer(&currentTime, &lcd);
WebServer webServer(NULL, &lcd);

char temp = 'A';

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  currentTime = 1553116366;
  writeString(&lcd, "Testing string display. This string seems to be wrapping around ok to the correct lines.");
  timer.start(1000);
  webServer.start(3000);
}

void loop()
{
  VariableTimedAction::updateActions();
}
