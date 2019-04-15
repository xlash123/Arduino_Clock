#ifndef COMMAND_H
#define COMMAND_H

class Command{
  public:
    char name[20];

    Command(const char *name);

    bool run(const char **args, int num);

};

#endif
