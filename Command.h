#ifndef COMMAND_H
#define COMMAND_H

#include "Alarms.h"
#include <string.h>

#define COM_SIZE 1

class Command{
  public:

    static const char *commands[];
    static bool (*funcs[])(const char**, int);

    static bool run(const char *name, const char **args, int n);

    static bool newalarm(const char **args, int n);

};

#endif
