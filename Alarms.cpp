#include "Alarms.h"

int Alarms::maxSize = 5;
time_t Alarms::lastTime = now();
Alarm ** Alarms::alarms = new Alarm*[Alarms::maxSize];

void Alarms::resize(){
  maxSize += 5;
  Alarm **temp = new Alarm*[maxSize];
  for(int i=0; i<maxSize-5; i++){
    temp[i] = alarms[i];
  }
  for(int i=maxSize-5; i<maxSize; i++){
    temp[i] = NULL;
  }
  delete [] alarms;
  alarms = temp;
}

int Alarms::getEmptyIndex(){
  for(int i=0; i<maxSize; i++){
    if(alarms[i] != NULL) return i;
  }
  return -1;
}

void Alarms::addAlarm(Alarm *al){
  for(int i=0; i<maxSize; i++){
    if(strcmp(alarms[i]->name, al->name) == 0){
      free(&al);
      return;
    }
  }
  int index = getEmptyIndex();
  if(index == -1){
    resize();
    index = maxSize-5;
  }
  alarms[index] = al;
  maxSize++;
}

void Alarms::removeAlarm(char *name){
  for(int i=0; i<maxSize; i++){
    if(strcmp(alarms[i]->name, name) == 0){
      alarms[i] = NULL;
    }
  }
}

void Alarms::saveAll(){
  SdFile file;
  file.open("alarms.dat", O_WRITE);
  if(file.isOpen()){
    file.write(&maxSize, sizeof(int));
    for(int i=0; i<maxSize; i++){
      void *al = alarms[i];
      saveAlarm(al);
    }
    file.close();
  }
}

void Alarms::saveAlarm(Alarm* al) {
    SdFile file;
  file.open("alarms.dat", O_WRITE);
  if(file.isOpen()){
    file.write(&maxSize, sizeof(int));
    //Save the first 3 strings
    for(int j=0; j<3; j++){  
      char *str = (char *) (al+sizeof(char*)*j);
      uint16_t len = strlen(str);
      file.write(&len, sizeof(uint16_t));
      file.write(str, len*sizeof(char));
    }
    file.write(al+3*sizeof(char*), sizeof(Alarm)-3*sizeof(char*));
    file.close();
  }
}

void Alarms::readAll(){
  SdFile file;
  file.open("alarms.dat");
  if(file.isOpen()){
    file.read(&maxSize, sizeof(int));
    delete [] alarms;
    alarms = new Alarm*[maxSize];
    for(int i=0; i<maxSize; i++){
      alarms[i] = (Alarm *) malloc(sizeof(Alarm));
      void *al = alarms[i];
      for(int j=0; j<3; j++){
        uint16_t len;
        file.read(&len, sizeof(uint16_t));
        char *str = (char *) malloc((len+1)*sizeof(char));
        file.read(str, sizeof(char)*len);
        str[len+1] = '\0';
        char *strS = (char *) (al+sizeof(char*)*j);
        strS =  str;
      }
      file.read(al+3*sizeof(char*), sizeof(Alarm)-3*sizeof(char*));
    }
    file.close();
  }
}

void Alarms::checkAlarms(time_t t){
  time_t currentTime = now();
  int day = weekday(currentTime) - 1;
  for(int i=0; i<maxSize; i++){
    Alarm alm = *alarms[i];
    time_t alarmTime = alm.time + elapsedSecsToday(currentTime);
    if((alm.repeat & (1 << day)) && alarmTime >= currentTime && alarmTime < lastTime){
      if (alm.skipCount == 0) {
        ring(&alm);
      }
      // If there are no more days left in the week for this alarm
      if (!(alm.repeat & (0xFF << (day + 1)))) {
        if (alm.skipCount == 0) {
          alm.skipCount = alm.skipInterval;
        } else {
          alm.skipCount--;
        }
      }
    }
  }
  lastTime = currentTime;
}

void Alarms::ring(Alarm *alm){
  
}

unsigned long Alarms::run(){
  time_t t = now();
  int sec = second(t);
  checkAlarms(t);
  return 60000-sec*1000;
}
