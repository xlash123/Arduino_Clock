#include "LCD.h"

LiquidCrystal_I2C * LCD::lcd;
char LCD::lcdDisp[ROWS*COLS+3];
int LCD::cursor;

LCD::LCD(LiquidCrystal_I2C &lcd){
  LCD::lcd = &lcd;
  lcd.init();
  lcd.backlight();
  clear();
}

void LCD::clear(){
  lcd->clear();
  setCursor(0);
  for(int i=0; i<LENGTH+1; i++){
    lcdDisp[i] = '\0';
  }
}

void LCD::writeString(const char *str, int row, int col){
  int len = strlen(str);
  setCursor(row*COLS + col%COLS);
  for(int i=0; i<len; i++){
    write(str[i]);
  }
  update();
}

void LCD::write(char c){
  if(cursor >= LENGTH) return;
  if(c != '\n'){
    lcdDisp[cursor] = c;
    cursor++;
  }else{
    cursor = cursor/COLS+COLS;
  }
}

void LCD::setCursor(int pos){
  cursor = pos;
}

void LCD::appendString(const char *str){
  int len = strlen(str);
  for(int i=0; i<len; i++){
    write(str[i]);
  }
  update();
}

void LCD::clearRow(int row){
  int temp = cursor;
  cursor = row*COLS;
  for(int i=0; i<COLS; i++) write(' ');
  cursor = temp;
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
