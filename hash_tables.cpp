#include "hash_tables.h"
#include "arguments.h"

template <class T>
HashTable<T>::HashTable(vector_list_collection<T> data_set, int k_value, int l_value, int R_value, int N_value, double C_value)
{

    table_size = data_set.size() / 16;
    vector_size = data_set[0].second.size();

    K = k_value;
    L = l_value;
    R = R_value;
    N = N_value;
    c = C_value;
}

template <class T>
T manhattan_distance(std::vector<T> x, std::vector<T> y)

{
    T result = 0;

    typename std::vector<T>::iterator it;
    typename std::vector<T>::iterator ti;

    for (it = x.begin(), ti = y.begin(); it != x.end(); ++it, ++ti)
    {
        result += abs(*it - *ti);
    }
    return result;
}

template <class T>
bool sortbysec(cand_img<T> &a, cand_img<T> &b)
{
    return (a.second < b.second);
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

template <class T>
std::vector<cand_img<T>> bruteforcelsh(vector_list_collection<T> data_set, image<T> query, int N)
{

    std::vector<cand_img<T>> best_imgs(N); //Vector with <Key:Image , Value:Distance>
    best_imgs.clear();

    image<T> temp_img; //Each image that we check

    T temp_dist; //Each distance that we check

    cand_img<T> temp_cand; //A pair of image and its distance

    for (int i = 0; i < data_set.size(); i++)
    {

        temp_img = data_set[i];
        temp_dist = manhattan_distance<T>(data_set[i].second, query.second);
        temp_cand = make_pair(temp_img, temp_dist);

        if (i <= N)
        { //If our list is not full

            if (i == N)
            {

                std::sort(best_imgs.begin(), best_imgs.end(), sortbysec<T>);

                if (temp_dist < best_imgs.back().second)
                { //If the temp. distance if lower than the last item of our candidates, then replace it

                    best_imgs[N - 1] = temp_cand; //If doesen't work, try vector.insert(vector.end(), x)
                    std::sort(best_imgs.begin(), best_imgs.end(), sortbysec<T>);
                }
            }

            else
                best_imgs.push_back(temp_cand);
        }

        else
        { //If our list is full, then we need to compare the elements

            if (temp_dist < best_imgs.back().second)
            { //If the temp. distance if lower than the last item of our candidates, then replace it

                best_imgs[N - 1] = temp_cand; //If doesen't work, try vector.insert(vector.end(), x)
                std::sort(best_imgs.begin(), best_imgs.end(), sortbysec<T>);
            }
        }
    }

    return best_imgs;
}

uint32_t swap_endian(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

void read_metadata(std::ifstream &file, uint32_t &magic_num, uint32_t &num_of_images, uint32_t &rows, uint32_t &columns, int size)
{ //Take the metadata (magic number, number of images, rows and columns)

    file.read(reinterpret_cast<char *>(&magic_num), size); //we are doing this type of cast bcz we are playing with bits, and bcz we want to convert: 0x00000803 -> 2051
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
    vector_list_collection<T> data;

    std::ifstream file;                                    // Stream class to read from files
    file.open(file_name, std::ios::in | std::ios::binary); // in(open for input operations), binary(open in binary mode)

    if (file.is_open()) //If the file has successfully opened
    {
        uint32_t magic_num = 0, num_of_images = 0, rows = 0, columns = 0; //uint32_t => unsigned byte, [ 0, 2^32 -1 ]. That's the type of the magic num, noi,r, c, according to the assignment.

        read_metadata(file, magic_num, num_of_images, rows, columns, 4);

        for (uint32_t i = 0; i < num_of_images; ++i)
        {
            data.push_back(std::pair<int, std::vector<T>>(i, std::vector<T>()));

            for (uint32_t r = 0; r < rows; ++r)
            {
                for (uint32_t c = 0; c < columns; ++c)
                {
                    unsigned char temp = 0;
                    file.read((char *)&temp, sizeof(temp));
                    data[i].second.push_back((int)temp); //each pixel of image (Question(?)) => Why not (char) temp? Each pixel is 0 until 255. Int is a waste of memory(?)
                }
            }
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
bool handle_conflicts(vector_list_collection<T> &centroids, image<T> &image, clusters<T> &i2c, int dist)
{
   
    for (auto it = i2c.begin(); it != i2c.end(); ++it) //For all vectors of centroids
    {
         int index = 0;
        for (auto ti = (it->second).begin(); ti < (it->second).end(); ++ti) //Check if given image is part of any cluster
        {
            if (image.first == ti->first) //If yes, calculate the Manhattan distance between cluster's centroid and image
            {
                if (manhattan_distance(image.second, (it->first).second) > dist)
                {
                    (it->second).erase((it->second).begin() + index); //If distance is larger than given distance erase image from current cluster
                    return true;
                }
                else
                {
                    return false;
                }
            }
            index++;
        }
        
    }
    return true; //If it reaches that point, given image wasn't part of any cluster yet
}

template class HashTable<int>;
template class HashTable<double>;
template image<int> bruteforce(vector_list_collection<int>, image<int>, int &);
template image<double> bruteforce(vector_list_collection<double>, image<double>, double &);
template std::vector<cand_img<int>> bruteforcelsh(vector_list_collection<int>, image<int>, int);
template std::vector<cand_img<double>> bruteforcelsh(vector_list_collection<double>, image<double>, int);
template bool sortbysec<int>(cand_img<int> &a, cand_img<int> &b);
template bool sortbysec<double>(cand_img<double> &a, cand_img<double> &b);
template bool handle_conflicts<int>(vector_list_collection<int> &, image<int> &, clusters<int> &, int);
template bool handle_conflicts<double>(vector_list_collection<double> &, image<double> &, clusters<double> &, int);
