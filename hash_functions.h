#ifndef HASH_FUNCTIONS
#define HASH_FUNCTIONS

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <bits/stdc++.h>
#include <list>
#include <algorithm>

template <class T>
class HFunction
{
public:
    HFunction(int, int);
    int apply(std::list<T>);

protected:
    std::vector<double> s;
    float w;
    int M;
    unsigned int m;
    std::vector<int> m_power;

    void vector_init(std::vector<double> &, int);
};

template <class T>
class f_Function
{
public:
    f_Function(int, int);
    // f_Function( std::vector<HFunction<T> *>);
    std::string apply(std::list<T>);

private:
    std::map<int, std::string> dict;
    HFunction<T>* hf;
    std::mt19937 generator;
     std::vector<HFunction<T> *> h_functions;
};

template <class T>
class GFunction
{
public:
    GFunction(std::vector<HFunction<T> *>);
    int apply(std::list<T>);

private:
    std::vector<HFunction<T> *> functions;
};

bool hammingDist(std::string str1, std::string str2);
#endif