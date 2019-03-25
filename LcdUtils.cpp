#include "LcdUtils.h"

void writeString(LiquidCrystal_I2C *lcd, const char *str){
  int len = strlen(str);
  for(int r=0; r<ROWS;){
    for(int c=0; c<COLS; c++){
      int i = c+r*COLS;
      if(i < len) lcd->write(str[i]);
    }
    if(r==0) r=2;
    else if(r==2) r=1;
    else if(r==1) r=3;
    else r++;
  }
}
