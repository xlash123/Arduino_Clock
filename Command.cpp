#include "Command.h"

const char *Command::commands[] = {"newalarm", "getalarms"};
bool (*Command::funcs[])(const char**, size_t, EthernetClient*) = {Command::newalarm, Command::getalarms};

bool Command::run(const char *cmd, size_t n, EthernetClient *client) {
  return true;
}

bool Command::run(const char *name, const char **args, size_t n, EthernetClient *res) {
  for(size_t i=0; i<COM_SIZE; i++){
    if(strcmp(name, commands[i]) == 0){
      return (*funcs[i])(args, n, res);
    }
  }
}

//Name, message, time, repeat, skipInterval, sound, persistent
bool Command::newalarm(const char** args, size_t n, EthernetClient *res){
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

bool Command::getalarms(const char** args, size_t n, EthernetClient *res) {

  return true;
}
