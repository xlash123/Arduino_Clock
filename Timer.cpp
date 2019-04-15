#include <VariableTimedAction.h>
#include <EthernetUdp.h>
#include <Timezone.h>

#include "LCD.h"

class Timer : public VariableTimedAction {
  public:
    Timer(){
      LCD::clearRow(1);
      LCD::writeString("Starting timer...", 1);
    }
  
  private:
    time_t lastUpdate = 0;

    unsigned long run(){
      time_t currentTime = now();
      int h = hour(currentTime);
      int m = minute(currentTime);
      int s = second(currentTime);
      LCD::setCursor(0);
      LCD::write('0'+h/10);
      LCD::write('0'+h%10);
      LCD::write(':');
      LCD::write('0'+m/10);
      LCD::write('0'+m%10);
      LCD::write(':');
      LCD::write('0'+s/10);
      LCD::write('0'+s%10);
      LCD::update();
      
      return 0;
    }
    
};
