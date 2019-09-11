#include <VariableTimedAction.h>
#include <EthernetUdp.h>
#include <Timezone.h>
#include <SevenSegmentTM1637.h>

#include "LCD.h"

class Timer : public VariableTimedAction {
  public:
    Timer(SevenSegmentTM1637* disp){
      LCD::clearRow(1);
      LCD::writeString("Starting timer...", 1);
      this->display = disp;
    }
  
  private:
    time_t lastUpdate = 0;
    SevenSegmentTM1637* display;

    unsigned long run(){
      time_t currentTime = now();
      int h = hour(currentTime);
      int m = minute(currentTime);
      int s = second(currentTime);

      uint8_t segments[] = {h/10, h%10, m/10, m%10};
      
      LCD::setCursor(0);
      LCD::write('0'+segments[0]);
      LCD::write('0'+segments[1]);
      LCD::write(':');
      LCD::write('0'+segments[2]);
      LCD::write('0'+segments[3]);
      LCD::write(':');
      LCD::write('0'+s/10);
      LCD::write('0'+s%10);
      LCD::update();

      for (int i=0; i<4; i++) {
        display->setCursor(0, i);
        display->print(segments[i]);
      }
      
      return 0;
    }
    
};
