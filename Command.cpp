#include "Command.h"

char rfc3986[256] = {0xFF};

char* encode(const char *s)
{
  char* enc = (char*) malloc(sizeof(char) * strlen(s) + 1);
  if (rfc3986[0] == 0xFF) {
    for (uint8_t i = 0; i < 256; i++) {
      rfc3986[i] = isalnum(i)||i == '~'||i == '-'||i == '.'||i == '_' ? i : 0;
    }
  }
  for (; *s; s++) {
    if (rfc3986[*s]) sprintf(enc, "%c", rfc3986[*s]);
    else sprintf(enc, "%%%02X", *s);
    while (*++enc);
  }
  return enc;
}

inline int ishex(int x)
{
  return  (x >= '0' && x <= '9')  ||
    (x >= 'a' && x <= 'f')  ||
    (x >= 'A' && x <= 'F');
}
 
char* decode(const char *s)
{
  char* dec = (char*) malloc(sizeof(char) * strlen(s) + 1);
  char *o;
  const char *end = s + strlen(s);
  int c;
 
  for (o = dec; s <= end; o++) {
    c = *s++;
    if (c == '+') c = ' ';
    else if (c == '%' && (  !ishex(*s++)  ||
          !ishex(*s++)  ||
          !sscanf(s - 2, "%2x", &c))) {
            free(dec);
            return -1; 
          }
    if (dec) *o = c;
  }
 
  return dec;
}

const char *Command::commands[] = {"newalarm", "getalarms"};
void (*Command::funcs[])(const char**, size_t, EthernetClient*) = {Command::newalarm, Command::getalarms};

// Run command string. Arguments may be URI encoded
void Command::run(char *cmd, size_t n, EthernetClient *client) {
  // Break down into command name and array of arguments
  char *name = strtok(cmd, " ");
  char **args;
  if (name) {
    char* arg = name;
    while ((arg = strtok(NULL, " ")) != NULL){
      
    }
  }
}

// Run command name with arguments.
void Command::run(const char *name, const char **args, size_t n, EthernetClient *res) {
  for(size_t i=0; i<COM_SIZE; i++){
    if(strcmp(name, commands[i]) == 0){
      (*funcs[i])(args, n, res);
    }
  }
}

//Name, note, time, repeat, skipInterval, sound, persistent
void Command::newalarm(const char** args, size_t n, EthernetClient *res){
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
}

void writeProp(const char* name, const char* prop, EthernetClient *res) {
  char str[100] = "";
  strcpy(str, "\"");
  strcpy(str, name);
  strcpy(str, "\": \"");
  char* propEnc = encode(prop);
  strcpy(str, propEnc);
  strcpy(str, "\",");
  
  res->write(str);
  free(propEnc);
}

void Command::getalarms(const char** args, size_t n, EthernetClient *res) {
  res->write("[");
  bool first = true;
  for (size_t i=0; i<Alarms::maxSize; i++) {
    Alarm* alm = Alarms::alarms[i];
    if (alm != NULL) {
      if (!first) {
        res->write(",");
        first = true;
      }
      res->write("{");
      char numStr[20] = "";
      
      writeProp("name", alm->name, res);
      writeProp("note", alm->note, res);
      sprintf(numStr, "%d", alm->time);
      writeProp("time", numStr, res);
      sprintf(numStr, "%d", alm->repeat);
      writeProp("repeat", numStr, res);
      sprintf(numStr, "%d", alm->skipInterval);
      writeProp("skipInterval", numStr, res);
      writeProp("sound", alm->soundFile, res);
      writeProp("persistent", alm->persistent ? "true" : "false", res);

      res->write("}");
    }
  }
  res->write("]");
}
