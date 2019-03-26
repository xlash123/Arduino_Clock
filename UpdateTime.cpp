#include <VariableTimedAction.h>
#include <time.h>

class UpdateTime : public VariableTimedAction {
  public:
    UpdateTime(time_t *currentTime){ //Also accept method for internet connection
      this->currentTime = currentTime;
    }

  private:
    time_t *currentTime;

    unsigned long run(){
      *currentTime = 42069; //Get time from the network

      return 0;
    }
};
