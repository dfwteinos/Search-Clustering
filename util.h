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
void lsh(std::string, std::string, int, int, int, int, double,std::string);

template <class T>
void cube(std::string, std::string, int, int, int, int , double ,int, std::string);

template <class T>
double cost_function(double ,double ,T ,T);

///////////////////////////////////     Cluster functions    ////////////////////////////////////

template <class T>
void clustering(std::string, std::string, std::string, int, std::string, int, int, double, int, double, int, int);

template <class T>
void k_means(vector_list_collection<T>, vector_list_collection<T>&, int);

template <class T>
void find_nearest_centroid(vector_list_collection<T>, distances<T>&, image<T>, int, int&);

template <class T>
void normalization_Distances(distances<T> , std::vector<double>& , int);

void compute_probs(std::vector<double> , std::vector<double>&);

void choose_random_x(std::vector<double> , double &);

template <class T>
void Lloyds(vector_list_collection<T>, std::vector<cluster<T>>& , vector_list_collection<T>);

template <class T>
void k_Medians(std::vector<cluster<T>> , vector_list_collection<T>&, int);

template <class T>
T computeMedian(std::vector<T>);

template <class T>
double checkVariation(vector_list_collection<T>, vector_list_collection<T>);

template <class T>
void Silhouette(std::vector<cluster<T>>, std::vector<double>&);

template <class T>
void return_index(std::vector<T>, std::vector<cluster<T>>, int&, int);

template <class T>
void performClustering(std::string , vector_list_collection<T> , std::vector<cluster<T>>& , vector_list_collection<T> , int , int , double , int , double , int , int);

template <class T>
void clusteringResults(std::string, std::string, std::string, std::vector<cluster<T>>, std::vector<double>, double);


#endif //UTIL