#include "Command.h"
#include <string.h>

Command::Command(const char *name){
  strcpy(this->name, name);
}
