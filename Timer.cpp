#include  <LiquidCrystal_I2C.h>
#include <time.h>
#include <VariableTimedAction.h>

class Timer : public VariableTimedAction {
  public:
    Timer(time_t *currentTime, LiquidCrystal_I2C *lcd){
      this->currentTime = currentTime;
      this->lcd = lcd;
    }
  
    int getCount(){
      return count;
    }
  
  private:
    int count = 0;
    time_t *currentTime;
    LiquidCrystal_I2C *lcd;

    unsigned long run(){
      count++;
      int hour = (*currentTime/(60*60))%24-4;
      int minute = (*currentTime/60)%60;
      int sec = *currentTime%60;
      lcd->clear();
      lcd->write('0'+hour/10);
      lcd->write('0'+hour%10);
      lcd->write(':');
      lcd->write('0'+minute/10);
      lcd->write('0'+minute%10);
      lcd->write(':');
      lcd->write('0'+sec/10);
      lcd->write('0'+sec%10);
      (*currentTime)++;
      return 0;
    }
};

