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
LSH<T>::LSH(vector_list_collection<T> data_set, int k_value, int l_value, int R)
    : HashTable<T>(data_set, k_value, l_value, R)
{
    fill_table(data_set);
}

template <class T>
void LSH<T>::fill_table(vector_list_collection<T> data_set)
{
    std::cout << "in fill table " << std::endl;
    int index;
    for (size_t q = 0; q < data_set.size(); q++)
    {
        for (size_t i = 0; i < this->L; i++)
        {
            index = this->g_functions[i]->apply(data_set[q].second) % this->table_size;
            this->tables[i][index].push_back(data_set[q]);
        }
    }
    std::cout << "exiting fill_table" << std::endl;
    return;
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
image<T> LSH<T>::approximateNN(image<T> query, T &res)
{
    image<T> best;
    T best_distance = std::numeric_limits<T>::max();
    for (size_t i = 0; i < this->L; i++)
    {
        int full_index = this->g_functions[i]->apply(query.second);
        int index = full_index % this->table_size;
        int count = 0;
        for (size_t j = 0; j < this->tables[i][index].size(); j++)
        {
            int data_index = this->g_functions[i]->apply(this->tables[i][index][j].second);
            if (data_index == full_index)
                count++;
            if (count > 10 * this->L)
            {
                break;
            }
            T distance = manhattan_distance<T>(query.second, this->tables[i][index][j].second);
            if (distance < best_distance)
            {
                best = this->tables[i][index][j];
                best_distance = distance;
            }
            if (distance <= this->R)
            {
                this->neighbours.insert(this->tables[i][index][j]);
            }
        }
    }
    res = best_distance;

    return best;
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

        for (uint32_t i = 0; i < num_of_images; ++i)
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

template <class T>
HyperCube<T>::HyperCube(vector_list_collection<T> t, int k, int L, int R, int M, int probes)
    : HashTable<T>(t, k, L, R), M(M), probes(probes) { fill_table(t); }

template <class T>
void HyperCube<T>::fill_table(vector_list_collection<T> data_set)
{
    std::cout << "in hypercube's fill table " << std::endl;
    T index;
    for (size_t q = 0; q < data_set.size(); q++)
    {
        std::string temp;
        for (size_t i = 0; i < this->L; i++)
        {
            index = this->g_functions[i]->apply(data_set[q].second);
            if (dict.find(index) == dict.end())
            {
                this->dict.insert(std::make_pair(index, std::to_string((rand() > RAND_MAX / 2) ? 0 : 1)));
            }
            temp += dict[index];
        }
        f.insert(make_pair(temp, data_set[q]));
    }
}

template <class T>
image<T> HyperCube<T>::approximateNN(image<T> query, T &res)
{
    image<T> best;
    if (this->probes > this->f.size())
        this->probes = this->f.size();
    int best_distance = std::numeric_limits<int>::max();
    std::string temp;
    for (size_t i = 0; i < this->L; i++)
    {
        int index = this->g_functions[i]->apply(query.second) % this->table_size;
        if (dict.find(index) == dict.end())
        {
            this->dict.insert(std::make_pair(index, std::to_string((rand() > RAND_MAX / 2) ? 0 : 1)));
        }
        temp += dict[index];
    }
    int count = 0, diff = 0;
    while ((count < this->probes))
    {
        int q = 0;
        for (auto it = f.begin(); it != f.end(); ++it)
        {
            if (hammingDist(temp, it->first, diff))
            {
                count++;
                q = 0;
                auto curr = f.equal_range(it->first);
                for (auto ti = curr.first; ti != curr.second; ++ti)
                {
                    q++;
                    int distance = manhattan_distance<T>(query.second, ti->second.second);
                    if (distance < best_distance)
                    {
                        best = ti->second;
                        best_distance = distance;
                    }
                    if (distance <= this->R)
                    {
                        this->neighbours.insert(ti->second);
                    }
                    if (q >= this->M)
                        break;
                }
                if (q >= this->M)
                    break;
            }
            if ((q >= this->M) || (count >= this->probes))
                break;
        }

        diff++;
    }
    res = best_distance;
    return best;
}

template class HyperCube<int>;
template class LSH<int>;
template class HashTable<int>;
template class HyperCube<double>;
template class LSH<double>;
template class HashTable<double>;
template image<int> bruteforce(vector_list_collection<int>, image<int>, int &);
template image<double> bruteforce(vector_list_collection<double>, image<double>, double &);
