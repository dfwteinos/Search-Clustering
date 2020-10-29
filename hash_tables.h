#ifndef HASH_TABLES
#define HASH_TABLES

#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <bits/stdc++.h>
#include <algorithm>
#include <set>
#include <list>
#include <map>
#include <iterator>
#include "hash_functions.h"

template <typename T>
using image = std::pair<int, std::list<T>>;

template <typename T>
using vector_list_collection = std::vector<image<T>>;

template <class T>
class HashTable
{
public:
    HashTable(vector_list_collection<T>, int, int, int);
    virtual image<T> approximateNN(image<T>, T &){};
    ~HashTable();
    static vector_list_collection<T> vectorise_data(std::string file);
    void table_state();
    virtual void fill_table(vector_list_collection<T>){};
    std::vector<GFunction<T> *> get_GFunctions();
    void get_neighbours(std::ostream &);

public:
    int table_size;
    int vector_size;
    int L;
    int K;
    double R;
    std::set<image<T>> neighbours;
    std::vector<std::vector<vector_list_collection<T>>> tables;
    std::vector<GFunction<T> *> g_functions;
    std::vector<HFunction<T> *> h_functions;
};

template <class T>
class LSH : public HashTable<T>
{
public:
    LSH(vector_list_collection<T>, int, int, int);
    image<T> approximateNN(image<T>, T &);
    void fill_table(vector_list_collection<T>);
};

template <class T>
class HyperCube : public HashTable<T>
{
public:
    HyperCube(vector_list_collection<T>, int, int, int, int, int);
    image<T> approximateNN(image<T>, T &);

private:
    void fill_table(vector_list_collection<T>);
    std::map<std::string, vector_list_collection<T>> points;
    std::set<std::string> point_values;
    std::vector<f_Function<T> *> f_functions;
    int M;
    int probes;
};

template <class T>
T manhattan_distance(std::list<T> x, std::list<T> y);

template <class T>
image<T> bruteforce(vector_list_collection<T>, image<T>, T &);

#endif //HASH_TABLES
