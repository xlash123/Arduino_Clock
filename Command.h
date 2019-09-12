#ifndef COMMAND_H
#define COMMAND_H

#include "Alarms.h"
#include <Ethernet.h>
#include <string.h>

#define COM_SIZE 2

class Command{
  public:

    static const char *commands[];
    static bool (*funcs[])(const char**, size_t, EthernetClient*); // Array of functions that return bool

    static bool run(const char *cmd, size_t n, EthernetClient *res);
    static bool run(const char *name, const char **args, size_t n, EthernetClient *res); // Maps func array to explicit function

    // Also can edit alarms when given the same name
    static bool newalarm(const char **args, size_t n, EthernetClient *res);
    static bool getalarms(const char **args, size_t n, EthernetClient *res);

};

#endif
