#include "hash_tables.h"
#include "arguments.h"

template <class T>
HashTable<T>::HashTable(vector_list_collection<T> data_set, int k_value, int l_value, int R_value)
{
    table_size = data_set.size() / 8;
    vector_size = 790; //data_set[0].second.size();
    K = k_value;
    L = l_value;
    R = R_value;
    //Creating K h_functions
    for (ssize_t i = 0; i < K; i++)
    {
        h_functions.push_back(new HFunction<T>(vector_size, K));
    }

    //Creating L g_functions
    for (ssize_t i = 0; i < L; i++)
    {
        g_functions.push_back(new GFunction<T>(h_functions));
    }

    //Creating L hash_tables
    tables.resize(L);
    for (size_t i = 0; i < L; i++)
    {
        tables[i].resize(table_size);
    }
}

template <class T>
HashTable<T>::~HashTable()
{
    for (ssize_t i = 0; i < K; i++)
        delete h_functions[i];
    for (ssize_t i = 0; i < L; i++)
        delete g_functions[i];
}


template <class T>
void HashTable<T>::table_state()
{
    for (size_t i = 0; i < tables.size() - 1; i++)
    {
        std::cout << "Table " << i << std::endl;
        for (size_t j = 0; j < tables[i].size(); j++)
        {
            std::cout << "\tContainer " << j << " size " << tables[i][j].size() << std::endl;
        }
    }
}

template <class T>
T manhattan_distance(std::list<T> x, std::list<T> y)
{
    T result = 0;
    typename std::list<T>::iterator it;
    typename std::list<T>::iterator ti;
    for (it = x.begin(), ti = y.begin(); it != x.end(); ++it, ++ti)
    {
        result += abs(*it - *ti);
    }
    return result;
}

template <class T>
image<T> bruteforce(vector_list_collection<T> data_set, image<T> query, T &res)
{
    image<T> best;
    T best_distance = std::numeric_limits<T>::max();
    T current;
    for (ssize_t i = 0; i < data_set.size(); i++)
    {
        current = manhattan_distance<T>(data_set[i].second, query.second);
        if (current < best_distance)
        {
            best_distance = current;
            best = data_set[i];
        }
    }
    res = best_distance;
    return best;
}

uint32_t swap_endian(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

void read_metadata(std::ifstream &file, uint32_t &magic_num, uint32_t &num_of_images, uint32_t &rows, uint32_t &columns, int size)
{ //Take the metadata (magic number, number of images, rows and columns)

    file.read(reinterpret_cast<char *>(&magic_num), size); //we are doing this type of cast bcz we are playing with bits, and bcz we want to convet: 0x00000803 -> 2051
    magic_num = swap_endian(magic_num);
    std::cout << "magic num: " << magic_num << std::endl;

    file.read(reinterpret_cast<char *>(&num_of_images), size);
    num_of_images = swap_endian(num_of_images);
    std::cout << "#images: " << num_of_images << std::endl;

    file.read(reinterpret_cast<char *>(&rows), size);
    rows = swap_endian(rows);
    std::cout << "#rows: " << rows << std::endl;

    file.read(reinterpret_cast<char *>(&columns), size);
    columns = swap_endian(columns);
    std::cout << "#columns: " << columns << std::endl;
}

template <class T>
vector_list_collection<T> HashTable<T>::vectorise_data(std::string file_name)
{
    // std::pair<double, vector_list_collection<T>> data;

    vector_list_collection<T> data;

    std::ifstream file;                                    //Stream class to read from files
    file.open(file_name, std::ios::in | std::ios::binary); // in(open for input operations), binary(open in binary mode)

    if (file.is_open()) //If the file has successfully opened
    {
        uint32_t magic_num = 0, num_of_images = 0, rows = 0, columns = 0; //uint32_t => unsigned bit, [ 0, 2^32 -1 ]. That's the type of the magic num, noi,r, c, according to the assignment.

        read_metadata(file, magic_num, num_of_images, rows, columns, 4);

        for (uint32_t i = 0; i < num_of_images/20; ++i)
        {
            data.push_back(std::pair<int, std::list<T>>(i, std::list<T>()));

            for (uint32_t r = 0; r < rows; ++r)
            {
                for (uint32_t c = 0; c < columns; ++c)
                {
                    unsigned char temp = 0;
                    file.read((char *)&temp, sizeof(temp));
                    data[i].second.push_back((int)temp); //each pixel of image
                }
            }

            // data.push_back(std::pair<std::string, std::vector<T>>("1", current)); //all elements
        }
        std::cout << "done reading files" << std::endl;
    }
    else
    {
        std::cout << "Something went wrong while reading the file " << file_name << std::endl;
        exit(EXIT_FAILURE);
    }

    return data;
}

template <class T>
void HashTable<T>::get_neighbours(std::ostream &my_file)
{
    if (this->R != 0)
    {
        for (auto it = this->neighbours.begin(); it != this->neighbours.end(); it++)
        {
            my_file << (*it).first << std::endl;
        }
    }
    this->neighbours.clear();
    return;
}

template class HashTable<int>;
template class HashTable<double>;
template image<int> bruteforce(vector_list_collection<int>, image<int>, int &);
template image<double> bruteforce(vector_list_collection<double>, image<double>, double &);
