#include "util.h"
using namespace std::chrono; 

template <class T>
double cost_function(double t_lsh, double t_true, T mean_lsh, T mean_true){

    double meanTime; 

    if( (double) t_true){
    
        meanTime = (t_lsh/t_true);
    }
    
    T acc = abs(mean_lsh - mean_true);

    std::cout << "acc: " << acc << " ,and time: " << meanTime << std::endl;

    return (double)(acc*meanTime);                                        

}


template <class T>
void lsh(std::string input_file, std::string query_file, int k, int L, int R, int N, double c, std::string output)
{
    std::clock_t start;

    //Brute and lsh duration
    double brute_dur, lsh_dur, average;                                         //hpc_dur, average; => We don't need hpc_dur in lsh

    //brute and lsh instances
    image<T> brute_res, lsh_res;                                                //, hpc_res;        => We don't need hpc_res in lsh
    
    //brute and lsh distance
    //Question(?) No diffrence between distance and type of pixel(?) Maybe we should change that

    T brute_dis, lsh_dis;                                                       // , hpc_dis;       => We don't need hps_dis in lsh
    double max = 0.0;
    

    // auto starti = std::chrono::steady_clock::now();
    
    vector_list_collection<T> input = HashTable<T>::vectorise_data(input_file);
    
    // auto stopi = std::chrono::steady_clock::now();
    // std::chrono::duration<double> elapsed_seconds = stopi-starti;
    // std::cout << elapsed_seconds.count() << std::endl;

    vector_list_collection<T> query = HashTable<T>::vectorise_data(query_file);

    std::ofstream myfile;
    myfile.open(output);

    T e_sum = 0, a_sum = 0;

    LSH<T> lsh(input, k, L, R, N, c);

    std::vector < cand_img <T> > bruteres;              //brute force vector
    std::vector < cand_img <T> > lshres;                //lsh vector
    std::vector < cand_img <T> > rsres;                 //range search vector

    T d_lsh = 0, d_true = 0;
    double t_lsh = 0, t_true = 0;

    // for (size_t i = 0; i < query.size(); i++)

    for (size_t i = 0; i < 5; i++)
    {
        std::cout << i << std::endl;
        start = std::clock();
        // brute_res = bruteforce<T>(input, query[i], brute_dis);
        bruteres = bruteforcelsh<T>(input, query[i], N);
        brute_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        
        start = std::clock();
        lshres = lsh.aNNeighbours(query[i] , N, rsres);
        lsh_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        // start = std::clock();
        myfile << "Query: " << query[i].first << std::endl;

        for(size_t j = 0; j < lshres.size(); j++ ){                   //For Each query point 

            myfile << "Nearest Neighbor-"<< j << ": " << (lshres[j].first).first << std::endl;
            myfile << "distanceLSH: " << lshres[j].second << std::endl;
            myfile << "distanceTrue: " << bruteres[j].second << std::endl;

            d_lsh   += lshres[j].second;
            d_true  += bruteres[j].second;
        }

        myfile << "R-near neighbors:" << std::endl;
        
        for (size_t r = 0 ; r < rsres.size(); r++){

            myfile << "image_number: " << (rsres[r].first).first << " ,distance: " << rsres[r].second << std::endl; 
        }
        
        myfile << "tLSH: " << lsh_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl << "\n";

        t_lsh   += lsh_dur;
        t_true  += brute_dur;

        // lsh_res = lsh.approximateNN(query[i], lsh_dis);
        // lshres = lsh.aNNeighbours(query[i] , N);
        // slsh_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        // myfile << "Query: " << query[i].first << std::endl;
        // myfile << "Nearest Neighbor: " << lsh_res.first << std::endl;
        // myfile << "DistanceLSH: " << lsh_dis << std::endl;
        // myfile << "DistanceTrue: " << brute_dis << std::endl;
        // myfile << "tLSH: " << lsh_dur << std::endl;
        // myfile << "tTrue: " << brute_dur << std::endl;
        // lsh.get_neighbours(myfile);

        // if (max < (double)lsh_dis / brute_dis)
            // max = (double)lsh_dis / brute_dis;
        // average += lsh_dur;
        // e_sum += brute_dis;
        // a_sum += lsh_dis;
    }

    T sum = 5;
    // T sum = query.size();
    myfile << "Cost is: " << (double) cost_function( t_lsh/sum, t_true/sum, (d_lsh/sum)/3, (d_true/sum)/3 ) << std::endl;
    // myfile << "Average distance:" << (double)a_sum / e_sum << std::endl;
    // myfile << "Max distance:" << max << std::endl;
    // myfile << "Average time:" << average / query.size() << std::endl;
    myfile.close();
}

template <class T>
void cube(std::string input_file, std::string query_file, int k, int M, int R, int N, double c,int probes, std::string output)
{
    std::clock_t start;
    double brute_dur, lsh_dur, hpc_dur, average;

    image<T> brute_res, lsh_res, hpc_res;
    T brute_dis, lsh_dis, hpc_dis;
    double max = 0.0;

    // vector_list_collection<T> curr = HashTable<T>::vectorise_data(input_file);
    // double R = curr.first;
    vector_list_collection<T> input = HashTable<T>::vectorise_data(input_file);
    vector_list_collection<T> query = HashTable<T>::vectorise_data(query_file);

    std::ofstream myfile;
    myfile.open(output);

    T e_sum = 0, a_sum = 0;
    HyperCube<T> hpc(input, k, log(input.size()), R, N, M, probes, c);

    for (size_t i = 0; i < query.size(); i++)
    {
        start = std::clock();
        brute_res = bruteforce<T>(input, query[i], brute_dis);
        brute_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        start = std::clock();
        hpc_res = hpc.approximateNN(query[i], hpc_dis);
        hpc_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        myfile << "Query: " << query[i].first << std::endl;
        myfile << "Nearest Neighbor: " << hpc_res.first << std::endl;
        myfile << "DistanceHPC: " << hpc_dis << std::endl;
        myfile << "DistanceTrue: " << brute_dis << std::endl;
        myfile << "tHPC: " << hpc_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl;
        hpc.get_neighbours(myfile);
        if (max < (double)hpc_dis / brute_dis)
            max = (double)hpc_dis / brute_dis;
        average += hpc_dur;
        e_sum += brute_dis;
        a_sum += hpc_dis;
    }

    myfile << "Average distance:" << (double)a_sum / e_sum << std::endl;
    myfile << "Max distance:" << max << std::endl;
    myfile << "Average time:" << average / query.size() << std::endl;
    myfile.close();
}

//Question(?): Are these necessary? 

template void lsh<int>(std::string, std::string, int, int, int, int, double, std::string output);
template void lsh<double>(std::string, std::string, int, int, int, int, double, std::string output);
template void cube<int>(std::string, std::string, int, int, int, int, double ,int, std::string output);
template void cube<double>(std::string, std::string, int, int, int, int, double ,int, std::string output);
