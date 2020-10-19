#include "util.h"

template <class T>
void lsh(std::string input_file, std::string query_file, int k, int L, int R, std::string output)
{
    std::clock_t start;
    double brute_dur, lsh_dur, hpc_dur, average;

    image<T> brute_res, lsh_res, hpc_res;
    T brute_dis, lsh_dis, hpc_dis;
    double max = 0.0;
    
    vector_list_collection<T> input = HashTable<T>::vectorise_data(input_file);
    vector_list_collection<T> query = HashTable<T>::vectorise_data(query_file);

    std::ofstream myfile;
    myfile.open(output);

    T e_sum = 0, a_sum = 0;

    LSH<T> lsh(input, k, L, R);

    for (size_t i = 0; i < query.size(); i++)
    {
        start = std::clock();
        brute_res = bruteforce<T>(input, query[i], brute_dis);
        brute_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        start = std::clock();
        lsh_res = lsh.approximateNN(query[i], lsh_dis);
        lsh_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        myfile << "Query: " << query[i].first << std::endl;
        myfile << "Nearest Neighbor: " << lsh_res.first << std::endl;
        myfile << "DistanceLSH: " << lsh_dis << std::endl;
        myfile << "DistanceTrue: " << brute_dis << std::endl;
        myfile << "tLSH: " << lsh_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl;
        lsh.get_neighbours(myfile);
        if (max < (double)lsh_dis / brute_dis)
            max = (double)lsh_dis / brute_dis;
        average += lsh_dur;
        e_sum += brute_dis;
        a_sum += lsh_dis;
    }
    myfile << "Average distance:" << (double)a_sum / e_sum << std::endl;
    myfile << "Max distance:" << max << std::endl;
    myfile << "Average time:" << average / query.size() << std::endl;
    myfile.close();
}

template <class T>
void cube(std::string input_file, std::string query_file, int k, int M, int R, int probes, std::string output)
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
    HyperCube<T> hpc(input, k, log(input.size()), R, M, probes);

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

template void lsh<int>(std::string, std::string, int, int, int, std::string output);
template void lsh<double>(std::string, std::string, int, int, int, std::string output);
template void cube<int>(std::string, std::string, int, int, int, int, std::string output);
template void cube<double>(std::string, std::string, int, int, int, int, std::string output);
