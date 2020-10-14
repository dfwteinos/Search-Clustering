#include "util.h"

// uint32_t swap_endian(uint32_t val)
// {
//     val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
//     return (val << 16) | (val >> 16);
// }

// void read_file(std::string file_name)
// {
//     std::ifstream file;                                                             //Stream class to read from files
//     file.open(file_name, std::ios::in | std::ios::binary);                          // in(open for input operations), binary(open in binary mode)

//     if (file.is_open())                                                             //If the file has successfully opened
//     {
//         uint32_t magic_num = 0, num_of_images = 0, rows = 0, columns = 0;           //uint32_t => unsigned bit, [ 0, 2^32 -1 ]. That's the type of the magic num, noi,r, c, according to the assignment.

//         read_metadata(file, magic_num, num_of_images, rows, columns, 4);

//         for (uint32_t i = 0; i < num_of_images; ++i)
//         {
//             for (uint32_t r = 0; r < rows; ++r)
//             {
//                 for (uint32_t c = 0; c < columns; ++c)
//                 {
//                     unsigned char temp = 0;
//                     file.read((char *)&temp, sizeof(temp));
//                     // std::cout << (double) temp << std::endl;                     //We have only int input, we are not going to need (double) cast.
//                     // std::cout << (int) temp << std::endl;
//                 }
//             }
//         }
//         std::cout << "done reading files" << std::endl;  
//     }
//     else
//     {
//         std::cout << "Something went wrong while reading the file " << file_name << std::endl;
//         exit(EXIT_FAILURE);
//     }
// }

// void read_metadata(std::ifstream& file, uint32_t& magic_num, uint32_t& num_of_images, uint32_t& rows, uint32_t& columns,int size){              //Take the metadata (magic number, number of images, rows and columns)

//     file.read(reinterpret_cast<char*>(&magic_num), size);                                                                                       //we are doing this type of cast bcz we are playing with bits, and bcz we want to convet: 0x00000803 -> 2051
//     magic_num = swap_endian(magic_num);
//     std::cout << "magic num: " << magic_num << std::endl;

//     file.read(reinterpret_cast<char*>(&num_of_images), size);
//     num_of_images = swap_endian(num_of_images);
//     std::cout << "#images: " << num_of_images << std::endl;

//     file.read(reinterpret_cast<char*>(&rows), size);
//     rows = swap_endian(rows);
//     std::cout << "#rows: " << rows << std::endl;

//     file.read(reinterpret_cast<char*>(&columns), size);
//     columns = swap_endian(columns);
//     std::cout << "#columns: " << columns << std::endl;

// }  

template <class T>
void lsh(std::string input_file, std::string query_file, int k, int L, std::string output) {
    std::clock_t start;                     
    double brute_dur, lsh_dur, hpc_dur, average;
    
    id_vector<T> brute_res, lsh_res, hpc_res;
    T brute_dis, lsh_dis, hpc_dis;
    double max = 0.0;
    
    std::pair< double, id_vector_collection<T>> curr = HashTable<T>::vectorise_data(input_file);
    double R = curr.first;
    id_vector_collection<T> input = curr.second;
    id_vector_collection<T> query = HashTable<T>::vectorise_data(query_file).second;
    
     std::ofstream myfile;
     myfile.open(output);
    
    T e_sum = 0, a_sum = 0;

    LSH<T> lsh(input, k, L, R);

    for (size_t i = 0; i < query.size(); i++){
        start = std::clock();
        brute_res = bruteforce<T>(input, query[i], brute_dis);
        brute_dur = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        start = std::clock();
        lsh_res = lsh.approximateNN(query[i], lsh_dis);
        lsh_dur = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        myfile << "Query: " << query[i].first << std::endl;
        myfile << "Nearest Neighbor: " << lsh_res.first << std::endl;
        myfile << "DistanceLSH: " << lsh_dis << std::endl;
        myfile << "DistanceTrue: " << brute_dis << std::endl;
        myfile << "tLSH: " << lsh_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl;
        lsh.get_neighbours(myfile);
        if (max < (double) lsh_dis / brute_dis) max = (double) lsh_dis / brute_dis;
        average += lsh_dur; 
        e_sum += brute_dis;
        a_sum += lsh_dis; 
    }
    myfile << "Average distance:" << (double) a_sum/e_sum << std::endl;
    myfile << "Max distance:" << max << std::endl;
    myfile << "Average time:" << average / query.size() << std::endl;
    myfile.close();
}

template <class T>
void cube(std::string input_file, std::string query_file, int k, int M, int probes,std::string output) {
    std::clock_t start;                     
    double brute_dur, lsh_dur, hpc_dur, average;
    
    id_vector<T> brute_res, lsh_res, hpc_res;
    T brute_dis, lsh_dis, hpc_dis;
    double max = 0.0;
    
    std::pair< double, id_vector_collection<T>> curr = HashTable<T>::vectorise_data(input_file);
    double R = curr.first;
    id_vector_collection<T> input = curr.second;
    id_vector_collection<T> query = HashTable<T>::vectorise_data(query_file).second;
    
     std::ofstream myfile;
     myfile.open(output);
    
    T e_sum = 0, a_sum = 0;
    HyperCube<T> hpc(input, k, log(input.size()), R, M, probes);

    for (size_t i = 0; i < query.size(); i++){
        start = std::clock();
        brute_res = bruteforce<T>(input, query[i], brute_dis);
        brute_dur = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        start = std::clock();
        hpc_res = hpc.approximateNN(query[i], hpc_dis);
        hpc_dur = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

        myfile << "Query: " << query[i].first << std::endl;
        myfile << "Nearest Neighbor: " << hpc_res.first << std::endl;
        myfile << "DistanceHPC: " << hpc_dis << std::endl;
        myfile << "DistanceTrue: " << brute_dis << std::endl;
        myfile << "tHPC: " << hpc_dur << std::endl;
        myfile << "tTrue: " << brute_dur << std::endl; 
        hpc.get_neighbours(myfile);
        if (max < (double) hpc_dis / brute_dis) max = (double) hpc_dis / brute_dis;
        average += hpc_dur; 
        e_sum += brute_dis;
        a_sum += hpc_dis;
    }
                
    myfile << "Average distance:" << (double) a_sum/e_sum << std::endl;
    myfile << "Max distance:" << max << std::endl;
    myfile << "Average time:" << average / query.size() << std::endl;
    myfile.close();
}

template void lsh<int>(std::string, std::string, int, int, std::string output);
template void lsh<double>(std::string, std::string, int, int, std::string output);
template void cube<int>(std::string, std::string, int, int, int, std::string output);
template void cube<double>(std::string, std::string, int, int, int, std::string output);
