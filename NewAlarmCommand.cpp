#include "Command.h"
#include "Alarms.h"
#include <string.h>
#include <stdio.h>

class NewAlarmCommand : public Command{
  public:
    bool run(const char **args, int num){
      //Name, message, time, repeat, skipInterval, sound, persistent
      if(num < 7) return false;
      Alarm alm;
      alm.name = (char *) malloc(sizeof(char)*strlen(args[0])+1);
      strcpy(alm.name, args[0]);
      alm.note = (char *) malloc(sizeof(char)*strlen(args[1])+1);
      strcpy(alm.note, args[1]);
      sscanf(args[2], "%d", &alm.time);
      int repeat;
      sscanf(args[3], "%d", &repeat);
      alm.repeat = (uint8_t) repeat;
      sscanf(args[4], "%d", &alm.skipInterval);
      alm.soundFile = (char *) malloc(sizeof(char)*strlen(args[5])+1);
      strcpy(alm.soundFile, args[5]);
      alm.persistent = strcmp("true", args[6])==0;
    }
};
