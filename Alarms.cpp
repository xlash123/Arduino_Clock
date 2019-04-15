#include "Alarms.h"

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

void Alarms::addAlarm(Alarm &alarm){
  int index = getEmptyIndex();
  if(index == -1){
    resize();
    index = maxSize-5;
  }
  alarms[index] = &alarm;
  maxSize++;
}

void Alarms::removeAlarm(Alarm &alarm){
  for(int i=0; i<maxSize; i++){
    if(alarms[i] == &alarm){
      alarms[i] = NULL;
    }
  }
}

void Alarms::saveAll(){
  FILE *file = fopen("alarms.dat", "w");
  fwrite(&maxSize, sizeof(int), 1, file);
  for(int i=0; i<maxSize; i++){
    fwrite(alarms[i], sizeof(Alarm), 1, file);
  }
  fclose(file);
}

void Alarms::readAll(){
  FILE *file = fopen("alarms.dat", "r");
  fread(&maxSize, sizeof(int), 1, file);
  delete [] alarms;
  alarms = new Alarm*[Alarms::maxSize];
  for(int i=0; i<maxSize; i++){
    alarms[i] = (Alarm *) malloc(sizeof(Alarm));
    fread(alarms[i], sizeof(Alarm), 1, file);
  }
}

void Alarms::checkAlarms(time_t t){
  int h = hour(t);
  int m = minute(t);
  int sec = second(t);
  for(int i=0; i<maxSize; i++){
    Alarm alm = *alarms[i];
    if(!alm.rung && alm.time == 60*m + 60*60*h){
      ring(alm);
    }else alm.rung = false;
  }
}

void Alarms::ring(Alarm &alm){
  alm.rung = true;
  
}
