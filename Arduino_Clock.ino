#define SDFAT

#include <Wire.h>
#include <VariableTimedAction.h>
#include <SPI.h>
#include <SdFat.h>
#include <Timezone.h>
#include <TMRpcm.h>
#include <SevenSegmentTM1637.h>

#include "LCD.h"
#include "Timer.cpp"
#include "WebServer.cpp"
#include "Alarms.h"

#include <TimeLib.h>

SdFat SD;

TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  //UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   //UTC - 5 hours
Timezone est(usEDT, usEST);
EthernetUDP Udp;

TMRpcm audio;

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
    LCD::clear();
    LCD::writeString("Error:\nSD not accessible");
    Serial.println(F("SD Error"));
    error = true;
  }
  while(error){
    delay(1);
  }
  WebServer webServer(&est, &audio);
  SevenSegmentTM1637 sevSeg(2, 3);
  sevSeg.begin();
  sevSeg.setBacklight(10);
  sevSeg.init();
  sevSeg.setColonOn(true);
  Timer timer(&sevSeg);
  Alarms alarms;
  Udp.begin(8888);
  // Syncronized the time with the time server every 30 minutes
  setSyncProvider(&getTime);
  setSyncInterval(30*60);
  Serial.println(F("No errors"));
  LCD::clearRow(0);
  LCD::clearRow(1);
  timer.start(1000);
  webServer.start(1000);
  alarms.start(1000);
  audio.speakerPin = 11;
  audio.setVolume(5);
  audio.play("test.wav");
}

int buttonPressed = 0;

void loop()
{
  VariableTimedAction::updateActions();
  int buttonState = digitalRead(31);
  if (!buttonPressed && buttonState == HIGH) {
    buttonPressed++;
    if(audio.isPlaying()) audio.pause();
    else audio.play("test.wav");
  }
  if (buttonState == LOW) {
    buttonPressed = 0;  
  }
}

time_t getTime(){
  Serial.println("Querying time server...");
  //Send packet
  byte packetBuf[48] = {0};
  packetBuf[0] = 0b11100011;
  packetBuf[1] = 0;
  packetBuf[2] = 6;
  packetBuf[3] = 0xEC;
  packetBuf[12] = 49;
  packetBuf[13] = 0x4E;
  packetBuf[14] = 49;
  packetBuf[15] = 52;
  Udp.beginPacket("time.nist.gov", 123);
  Udp.write(packetBuf, 48);
  Udp.endPacket();

  const unsigned long seventyYears = 2208988800UL;

  //Wait for packet
  int timeout = 0, offset = millis();
  while(!Udp.parsePacket() && timeout < 5000){
    delay(10);
    timeout += 10;
  }
  if(timeout >= 5000){
    Serial.println("No response");
    return 0;
  }
  Udp.read(packetBuf, 48);
  unsigned long highWord = word(packetBuf[40], packetBuf[41]);
  unsigned long lowWord = word(packetBuf[42], packetBuf[43]);
  time_t newTime = (((highWord << 16) | lowWord)) - seventyYears;
  Serial.print("UTC Time is: ");
  Serial.println(newTime);
  return est.toLocal(newTime) + (millis()-offset)/1000;
}
