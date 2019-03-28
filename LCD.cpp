#include "LCD.h"

LiquidCrystal_I2C * LCD::lcd;
char LCD::lcdDisp[ROWS*COLS+3];

LCD::LCD(LiquidCrystal_I2C &lcd){
  LCD::lcd = &lcd;
  lcd.init();
  lcd.backlight();
  clear();
}

void LCD::clear(){
  lcd->clear();
  for(int i=0; i<LENGTH+1; i++){
    lcdDisp[i] = '\0';
  }
}

void LCD::writeString(const char *str){
  clear();
  int len = strlen(str);
  for(int i=0; i<len; i++){
    write(str[i]);
  }
  update();
}

void LCD::write(char c){
  int len = strlen(lcdDisp);
  if(len >= LENGTH) return;
  if(c != '\n'){
    lcdDisp[len] = c;
    lcdDisp[len+1] = '\0';
  }else{
    for(int i=len; i<LENGTH && i<=(len%COLS+len*ROWS); i++){
      lcdDisp[i] = '\0';
    }
  }
}

void LCD::appendString(const char *str){
  int len = strlen(str);
  for(int i=0; i<len; i++){
    write(str[i]);
  }
  update();
}

void LCD::update(){
  for(int r=0; r<ROWS;){
    for(int c=0; c<COLS; c++){
      int i = r*COLS + c%COLS;
      if(lcdDisp[i] == '\0'){
        lcd->write(' ');
      }else lcd->write(lcdDisp[i]);
    }
    if(r==0) r=2;
    else if(r==2) r=1;
    else if(r==1) r=3;
    else r++;
  }
}
