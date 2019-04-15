#ifndef ALARMS_H
#define ALARMS_H

#include <VariableTimedAction.h>
#include <SD.h>
#include <stdio.h>
#include <Time.h>

typedef struct alarm{
  char *name, *note, *soundFile;
  uint8_t repeat; //Really just a byte
  int numRepeat, skipInterval, time; //time in seconds
  bool persistent, rung;
} Alarm;

class Alarms : public VariableTimedAction {
  public:
    static Alarm **alarms;
    static int maxSize;

    static void addAlarm(Alarm &alarm);
    static void removeAlarm(Alarm &alarm);
    static void ring(Alarm &alarm);
    static int getEmptyIndex();
    static void resize();

    static void saveAll();
    static void readAll();

    static void checkAlarms(time_t t);

    unsigned long run(){
      time_t t = now();
      int sec = second(t);
      checkAlarms(t);
      return 60000-sec*1000;
    }
    
};

#endif
