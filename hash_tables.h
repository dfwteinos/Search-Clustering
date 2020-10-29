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
#include <iterator>
#include "hash_functions.h"

template <typename T>
using image = std::pair< int, std::vector <T> >;                                           // pair: < (image_number) ,    the image it_self as a vector  >

template <typename T>
using vector_list_collection = std::vector< image<T> >;                                 // vector_list_collection: A vector of images

template <typename T>
using cand_img = std::pair< image<T>, T >;                                              // pair collection of a candidate image and its distance from a query q

template <class T>
class HashTable {
    public:
        HashTable(vector_list_collection<T>, int, int, int, int, double);
        virtual image<T> approximateNN(image<T>, T&){};
        ~HashTable();
        static vector_list_collection<T> vectorise_data(std::string file);              // static declaration in class makes this function accesible from non-class members
        void table_state();
        virtual void fill_table(vector_list_collection<T>) {};
        std::vector< GFunction<T>* > get_GFunctions();
        void get_neighbours(std::ostream&);

    protected:
        int table_size;
        int vector_size;
        int L;
        int K;
        int N;
        double R;
        double c;
        std::set<image<T> > neighbours;
        std::vector< std::vector< vector_list_collection<T> > > tables;
        std::vector< GFunction<T>* > g_functions;
        std::vector< HFunction<T>* > h_functions;
};

template <class T>
class LSH: public HashTable<T> {
    public:
        LSH(vector_list_collection<T>, int, int, int, int, double);
        image<T> approximateNN(image<T>, T&);
        void fill_table(vector_list_collection<T>);
        std::vector < cand_img<T> > aNNeighbours(image<T> , int , std::vector < cand_img<T>>&);                   //Returns K-Nearest Neighbours and their Distance from point q.
};

template <class T>
class HyperCube: public HashTable<T> {
    public:
        HyperCube(vector_list_collection<T>, int, int, int, int, int, int, double);
        image<T> approximateNN(image<T>, T&);
    
    private:
        void fill_table(vector_list_collection<T>);
        std::unordered_multimap<std::string,image<T>> f;
        std::unordered_map<T, std::string> dict;
        int M; 
        int probes;
   
};


template <class T>
image<T> bruteforce(vector_list_collection<T>, image<T>, T&);

template <class T>
std::vector < cand_img<T> > bruteforcelsh(vector_list_collection<T> , image<T>  , int );

template <class T>
bool sortbysec( cand_img<T> &a, cand_img<T> &b);


#endif //HASH_TABLES
