#ifndef UTIL
#define UTIL

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "hash_tables.h"

uint32_t swap_endian(uint32_t);                                                         //Convert from high endian to low endian. (and vice cersa?)

void read_file(std::string);                                                            //Read the given files (train and query)

void read_metadata(std::ifstream&, uint32_t&, uint32_t&, uint32_t&, uint32_t&, int);    //Take the metadata (magic number, number of images, rows and columns)
// uint32_t swap_endian(uint32_t);                                                         //Convert from high endian to low endian. (and vice cersa?)

// void read_file(std::string);                                                            //Read the given files (train and query)

// void read_metadata(std::ifstream&, uint32_t&, uint32_t&, uint32_t&, uint32_t&, int);    //Take the metadata (magic number, number of images, rows and columns)

template <class T>
void lsh(std::string input_file, std::string query_file, int k, int L, std::string output);

template <class T>
void cube(std::string input_file, std::string query_file, int k, int M, int probes, std::string output);

#endif //UTIL