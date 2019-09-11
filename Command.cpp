#include "Command.h"

const char *Command::commands[] = {"newalarm"};
bool (*Command::funcs[])(const char**, int) = {Command::newalarm};

bool Command::run(const char *name, const char **args, int n){
  for(int i=0; i<COM_SIZE; i++){
    if(strcmp(name, commands[i]) == 0){
      (*funcs[i])(args, n);
      break;
    }
  }
}

bool Command::newalarm(const char** args, int n){
  //Name, message, time, repeat, skipInterval, sound, persistent
  if(n < 7) return false;
  Alarm alm = *((Alarm *) malloc(sizeof(Alarm)));
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

  Alarms::addAlarm(&alm);

  return true;
}
