#ifndef ALARMS_H
#define ALARMS_H

#include <VariableTimedAction.h>
#include <SdFat.h>
#include <stdio.h>
#include <Time.h>
#include <string.h>

typedef struct alarm{
  char *name, *note, *soundFile; // Note - message to display on LCD
  uint8_t repeat; // bits 0-6 determine Sun-Sat repeat
  // numRepeat - how many times the alarm should be repeated
  // skipInterval - how many weekly instances of the alarm should be skipped
  // skipCount - the current counter of how many skips are left until repeated
  uint8_t numRepeat, skipInterval, skipCount;
  uint32_t time; // Time of day in seconds to run alarm
  // persistent - Alarm repeats until stopped
  bool persistent;
} Alarm;

class Alarms : public VariableTimedAction {
  public:
    static Alarm **alarms;
    static int maxSize;
    static time_t lastTime;

    static void addAlarm(Alarm *alarm);
    static void removeAlarm(char *name);
    static void ring(Alarm *alarm);
    static int getEmptyIndex();
    static void resize();

    static void saveAll();
    static void saveAlarm(Alarm* al);
    static void readAll();

    static void checkAlarms(time_t t);

    unsigned long run();
    
};

#endif
