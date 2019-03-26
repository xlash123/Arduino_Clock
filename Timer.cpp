#include <time.h>
#include <VariableTimedAction.h>

#include "LCD.h"

class Timer : public VariableTimedAction {
  public:
    Timer(time_t *currentTime){
      this->currentTime = currentTime;
    }
  
  private:
    time_t *currentTime;

    unsigned long run(){
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
};
