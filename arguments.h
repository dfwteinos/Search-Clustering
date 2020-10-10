#ifndef ARGUMENTS

#define ARGUMENTS

#include <string>
#include <string.h>

std::string get_operation(char *);

int get_parametres(int, char **,
                   std::string &,
                   std::string &,
                   std::string &,
                   int &, int &, int &, int&, int&,
                   double&);

#endif //ARGUMENTS