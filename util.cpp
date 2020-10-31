#include "util.h"

template <class T>
double cost_function(double t_lsh, double t_true, T mean_lsh, T mean_true)
{

    double meanTime;

    if ((double)t_true)
    {

        meanTime = (t_lsh / t_true);
    }

    T acc = abs(mean_lsh - mean_true);

    std::cout << "acc: " << acc << " ,and time: " << meanTime << std::endl;

    return (double)(acc * meanTime);
}

template <class T>
void lsh(std::string input_file, std::string query_file, int k, int L, int R, int N, double c, std::string output)
{
    std::clock_t start;

    //Brute and lsh duration
    double brute_dur, lsh_dur, average;

    //brute and lsh instances
    image<T> brute_res, lsh_res;

    //brute and lsh distance
    //Question(?) No diffrence between distance and type of pixel(?) Maybe we should change that

    T brute_dis, lsh_dis;
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

    std::vector<cand_img<T>> bruteres; //brute force vector
    std::vector<cand_img<T>> lshres;   //lsh vector
    std::vector<cand_img<T>> rsres;    //range search vector

    T d_lsh = 0, d_true = 0;
    double t_lsh = 0, t_true = 0;

    for (size_t i = 0; i < query.size(); i++)
    {
        std::cout << i << std::endl;
        start = std::clock();
        bruteres = bruteforcelsh<T>(input, query[i], N);
        brute_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        start = std::clock();
        lshres = lsh.aNNeighbours(query[i], N, rsres);
        lsh_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        myfile << "Query: " << query[i].first << std::endl;

        for (size_t j = 0; j < lshres.size(); j++)
        { //For Each query point

            myfile << "Nearest Neighbor-" << j << ": " << (lshres[j].first).first << std::endl;
            myfile << "distanceLSH: " << lshres[j].second << std::endl;
            myfile << "distanceTrue: " << bruteres[j].second << std::endl;

            d_lsh += lshres[j].second;
            d_true += bruteres[j].second;
        }

        myfile << "R-near neighbors:" << std::endl;

        for (size_t r = 0; r < rsres.size(); r++)
        {

            myfile << "image_number: " << (rsres[r].first).first << " ,distance: " << rsres[r].second << std::endl;
        }

        myfile << "tLSH: " << lsh_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl
               << "\n";

        t_lsh += lsh_dur;
        t_true += brute_dur;
    }

    T sum = 100;

    myfile << "Cost is: " << (double)cost_function(t_lsh / sum, t_true / sum, (d_lsh / sum) / 3, (d_true / sum) / 3) << std::endl;
    myfile.close();
}

template <class T>
void cube(std::string input_file, std::string query_file, int k, int M, int R, int N, double c, int probes, std::string output)
{
    std::clock_t start;

    //Brute and cube duration
    double brute_dur, hpc_dur, average;

    //Brute and cube distance
    T brute_dis, hpc_dis;
    double max = 0.0;

    // vector_list_collection<T> curr = HashTable<T>::vectorise_data(input_file);
    // double R = curr.first;
    vector_list_collection<T> input = HashTable<T>::vectorise_data(input_file);
    vector_list_collection<T> query = HashTable<T>::vectorise_data(query_file);

    std::ofstream myfile;
    myfile.open(output);

    T e_sum = 0, a_sum = 0;

    HyperCube<T> hpc(input, k, log(input.size()), R, N, M, probes, c);

    std::vector<cand_img<T>> bruteres; //brute force vector
    std::vector<cand_img<T>> hpcres;   //cube vector
    std::vector<cand_img<T>> rsres;    //range search vector

    T d_hpc = 0, d_true = 0;
    double t_hpc = 0, t_true = 0;

    for (size_t i = 0; i < query.size()/10; i++)
    {
        std::cout << i << std::endl;
        start = std::clock();
        bruteres = bruteforcelsh<T>(input, query[i], N);
        brute_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        start = std::clock();
        hpcres = hpc.aNNeighbours(query[i], N, rsres);
        hpc_dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        myfile << "Query: " << query[i].first << std::endl;

        for (size_t j = 0; j < hpcres.size(); j++)
        { //For Each query point

            myfile << "Nearest Neighbor-" << j << ": " << (hpcres[j].first).first << std::endl;
            myfile << "distanceHPC: " << hpcres[j].second << std::endl;
            myfile << "distanceTrue: " << hpcres[j].second << std::endl;

            d_hpc += hpcres[j].second;
            d_true += bruteres[j].second;
        }

        myfile << "R-near neighbors:" << std::endl;

        for (size_t r = 0; r < rsres.size(); r++)
        {

            myfile << "image_number: " << (rsres[r].first).first << " ,distance: " << rsres[r].second << std::endl;
        }

        myfile << "tHPC: " << hpc_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl
               << "\n";

        t_hpc += hpc_dur;
        t_true += brute_dur;
    }

    T sum = 100;

    myfile << "Cost is: " << (double)cost_function(t_hpc / sum, t_true / sum, (d_hpc / sum) / 3, (d_true / sum) / 3) << std::endl;
    myfile.close();
}

template void lsh<int>(std::string, std::string, int, int, int, int, double, std::string output);
template void lsh<double>(std::string, std::string, int, int, int, int, double, std::string output);
template void cube<int>(std::string, std::string, int, int, int, int, double, int, std::string output);
template void cube<double>(std::string, std::string, int, int, int, int, double, int, std::string output);
