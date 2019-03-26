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

    LCD(LiquidCrystal_I2C &lcd);

    static void clear();

    static void writeString(const char *str);

    static void write(char c);

    static void appendString(const char *str);

    static void update();
};

#endif
