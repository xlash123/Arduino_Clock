#ifndef COMMAND_H
#define COMMAND_H

#include "Alarms.h"
#include <Ethernet.h>
#include <string.h>

#define COM_SIZE 2

class Command{
  public:

    static const char *commands[];
    static void (*funcs[])(const char**, size_t, EthernetClient*); // Array of functions that return bool

    static void run(char *cmd, size_t n, EthernetClient *res);
    static void run(const char *name, const char **args, size_t n, EthernetClient *res); // Maps func array to explicit function

    // Also can edit alarms when given the same name
    static void newalarm(const char **args, size_t n, EthernetClient *res);
    static void getalarms(const char **args, size_t n, EthernetClient *res);

};

#endif
