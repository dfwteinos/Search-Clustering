#ifndef ARGUMENTS

#define ARGUMENTS

#include <iostream>
#include <string>
#include <ctime>
#include <string.h>

std::string get_operation(char *);                      //returns lsh or cube

int get_parameters(int, char **,                        //get input parameters from user
                    std::string,
                    std::string &,
                    std::string &,
                    std::string &,
                    int &, int &, int &, int&, int&,
                    double&);

void set_default_parameter(std::string,                //set default parameter k if user doesen't give it
                int,
                int &);

#endif //ARGUMENTS