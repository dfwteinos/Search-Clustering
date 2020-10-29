#ifndef UTIL
#define UTIL

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>
#include "hash_tables.h"
#include <cmath>        


template <class T>
void lsh(std::string, std::string, int, int, int, int, double ,std::string);

template <class T>
void cube(std::string, std::string, int, int, int, int , double ,int, std::string);

template <class T>
double cost_function(double ,double ,T ,T);

#endif //UTIL