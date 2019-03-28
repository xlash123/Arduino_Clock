#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include <string.h>

#define ROWS 4
#define COLS 20
#define LENGTH ROWS*COLS

class LCD{
  public:

    static LiquidCrystal_I2C *lcd;
    static char lcdDisp[ROWS*COLS+3];
    static int cursor;

    LCD(LiquidCrystal_I2C &lcd);

    static void clear();

    static void writeString(const char *str, int row=0, int col=0);

    static void write(char c);

    static void clearRow(int row);

    static void setCursor(int pos);

    static void appendString(const char *str);

    static void update();
};

#endif
