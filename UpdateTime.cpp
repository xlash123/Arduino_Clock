#include <VariableTimedAction.h>
#include <time.h>

class UpdateTime : public VariableTimedAction {
  public:
    UpdateTime(time_t *currentTime){ //Also accept method for internet connection
      this->currentTime = currentTime;
    }

    int getCount(){
      return count;
    }

  private:
    int count = 0;
    time_t *currentTime;

    unsigned long run(){
      count++;

      *currentTime = 42069; //Get time from the network

      return 0;
    }
};

