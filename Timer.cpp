#include <time.h>
#include <VariableTimedAction.h>
#include <EthernetUdp.h>

#include "LCD.h"

class Timer : public VariableTimedAction {
  public:
    Timer(time_t *currentTime){
      this->currentTime = currentTime;
      Udp.begin(8888);
    }
  
  private:
    time_t *currentTime;
    time_t lastUpdate = 0;
    EthernetUDP Udp;

    unsigned long run(){
      if(*currentTime - lastUpdate >= 2*60){
        lastUpdate = getTime();
        if(lastUpdate > 0){
          *currentTime = lastUpdate;
        }else lastUpdate = *currentTime - (2*60 - 10);
      }
      int hour = (*currentTime/(60*60))%24-4;
      int minute = (*currentTime/60)%60;
      int sec = *currentTime%60;
      LCD::clear();
      LCD::write('0'+hour/10);
      LCD::write('0'+hour%10);
      LCD::write(':');
      LCD::write('0'+minute/10);
      LCD::write('0'+minute%10);
      LCD::write(':');
      LCD::write('0'+sec/10);
      LCD::write('0'+sec%10);
      LCD::update();
      (*currentTime)++;
      return 0;
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

      //Wait for packet
      int timeout = 0, offset = millis();
      while(!Udp.parsePacket() && timeout < 2000){
        delay(10);
        timeout += 10;
      }
      if(timeout >= 2000){
        Serial.println("No response");
        return 0;
      }
      Udp.read(packetBuf, 48);
      unsigned long highWord = word(packetBuf[40], packetBuf[41]);
      unsigned long lowWord = word(packetBuf[42], packetBuf[43]);
      time_t newTime = ((highWord << 16) | lowWord) + (millis()-offset)/1000;
      Serial.print("Time is: ");
      Serial.println(newTime);
      return newTime;
    }
    
};
