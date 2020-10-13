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
#include "hash_functions.h"

template <typename T>
using id_vector = std::pair< std::string, std::vector<T> >;

template <typename T>
using id_vector_collection = std::vector< id_vector<T> >;


template <class T>
class HashTable {
    public:
        HashTable(id_vector_collection<T>, int, int, int);
        virtual id_vector<T> approximateNN(id_vector<T>, T&){};
        ~HashTable();
        static std::pair<double, id_vector_collection<T>> vectorise_data(std::string file);  
        void table_state();
        virtual void fill_table(id_vector_collection<T>) {};
        std::vector< GFunction<T>* > get_GFunctions();
        void get_neighbours(std::ostream&);

    protected:
        int table_size;
        int vector_size;
        int L;
        int K;
        double R;
        std::set<id_vector<T> > neighbours;
        std::vector< std::vector< id_vector_collection<T> > > tables;
        std::vector< GFunction<T>* > g_functions;
        std::vector< HFunction<T>* > h_functions;
};

template <class T>
class LSH: public HashTable<T> {
    public:
        LSH(id_vector_collection<T>, int, int, int);
        id_vector<T> approximateNN(id_vector<T>, T&);
        void fill_table(id_vector_collection<T>);
};

template <class T>
class HyperCube: public HashTable<T> {
    public:
        HyperCube(id_vector_collection<T>, int, int, int, int, int);
        id_vector<T> approximateNN(id_vector<T>, T&);
    
    private:
        void fill_table(id_vector_collection<T>);
        std::unordered_multimap<std::string,id_vector<T>> f;
        std::unordered_map<T, std::string> dict;
        int M; 
        int probes;
   
};


template <class T>
id_vector<T> bruteforce(id_vector_collection<T>, id_vector<T>, T&);

#endif //HASH_TABLES
