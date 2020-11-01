#include "hash_tables.h"
#include "arguments.h"

template <class T>
LSH<T>::LSH(vector_list_collection<T> data_set, int k_value, int l_value, int R, int N, double c)
    : HashTable<T>(data_set, k_value, l_value, R, N, c)
{

    //Creating K h_functions
    for (ssize_t i = 0; i < this->K; i++)
    {
        h_functions.push_back(new HFunction<T>(this->vector_size, this->K));
    }

    //Creating L g_functions
    for (ssize_t i = 0; i < this->L; i++)
    {
        g_functions.push_back(new GFunction<T>(h_functions));
    }

    //Creating L hash_tables
    //And n/16 buckets for each hash table
    tables.resize(this->L);
    for (size_t i = 0; i < this->L; i++)
    {
        tables[i].resize(this->table_size);
    }

    fill_table(data_set);
}

template <class T>
LSH<T>::~LSH()
{
    for (ssize_t i = 0; i < this->K; i++)
        delete h_functions[i];
    for (ssize_t i = 0; i < this->L; i++)
        delete g_functions[i];
}

template <class T>
void LSH<T>::fill_table(vector_list_collection<T> data_set)
{
    std::cout << "In Fill Table " << std::endl;
    int index;
    for (size_t q = 0; q < data_set.size(); q++)
    {
        for (size_t i = 0; i < this->L; i++)
        {
            index = (unsigned int)this->g_functions[i]->apply(data_set[q].second) % this->table_size;
            this->tables[i][index].push_back(data_set[q]);
        }
    }
    std::cout << "Exiting Fill Table" << std::endl;
    return;
}

template <class T>
std::vector<cand_img<T>> LSH<T>::aNNeighbours(image<T> query, int N, std::vector<cand_img<T>> &RS_vector)
{ //Returns K-Nearest Neighbours and their Distance from point q.

    std::vector<cand_img<T>> best_imgs(N); //Vector with pair <:Image , Distance>
    std::vector<cand_img<T>> RS_imgs;      //Vector with all the images that are inside our range search

    best_imgs.clear();

    image<T> temp_img; //Each image that we check

    T temp_dist; //Each distance that we check

    int retr_items = 0;

    cand_img<T> temp_cand;

    for (size_t i = 0; i < this->L; i++)
    { //For all the HashTables

        unsigned int full_index = this->g_functions[i]->apply(query.second); //Putting the query in a HashTable Bucket
        int index = full_index % this->table_size;
        int points = 0;

        for (size_t j = 0; j < this->tables[i][index].size(); j++)
        { //For all the elements inside this bucket in the i HashTable

            temp_img = this->tables[i][index][j];                                              //Each image at this specific bucket of the i HashTable
            temp_dist = manhattan_distance<T>(query.second, this->tables[i][index][j].second); //Each distance at this specific bucket of the i HashTable
            temp_cand = make_pair(temp_img, temp_dist);                                        //Each candidate of a pair of image(image itself and distance of this image from the query q)

            if (temp_dist <= (unsigned int)((this->R) * (this->c)))
                RS_imgs.push_back(temp_cand); //If the lsh distance is inside the (R-Ball) then push back this image

            if (best_imgs.size() <= N)
            { //If our list is not full

                retr_items++;

                if (best_imgs.size() == N)
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

                retr_items++;
                if (temp_dist < best_imgs.back().second)
                { //If the temp. distance if lower than the last item of our candidates, then replace it

                    best_imgs[N - 1] = temp_cand; //If doesen't work, try vector.insert(vector.end(), x)
                    std::sort(best_imgs.begin(), best_imgs.end(), sortbysec<T>);
                }
            }

            if ((retr_items >= 10 * this->L))
                break;
        }
    }

    RS_vector = RS_imgs;
    return best_imgs;
}

template <class T>
clusters<T> LSH<T>::reverse_assignment(vector_list_collection<T> centroids, vector_list_collection<T> images)
{

    clusters<T> i2c;
    std::map<int, vector_list_collection<T>> c_buckets; //To store centroids based on their place in the hash table

    for (auto ti = centroids.begin(); ti != centroids.end(); ++ti) //Initialize vector of centroids
    {
        vector_list_collection<T> v;
        i2c.push_back({(*ti), v});
    }

    for (auto ti = i2c.begin(); ti != i2c.end(); ++ti) //For every centroid in the vector, we start by checking at the same buckets
    {
        for (size_t i = 0; i < this->L; i++)
        {
            unsigned int full_index = this->g_functions[i]->apply((ti->first).second); //Find centroids position in a HashTable
            int index = full_index % this->table_size;

            if (c_buckets.find(index) == c_buckets.end()) //Keep centroid's position in map
            {
                vector_list_collection<T> temp;
                temp.push_back(ti->first);
                c_buckets.insert({index, temp});
            }
            else
            {
                c_buckets[index].push_back(ti->first);
            }

            for (size_t j = 0; j < this->tables[i][index].size(); j++)
            {
                if (this->tables[i][index][j].first != (ti->first).first)
                {
                    int temp_dist = manhattan_distance((ti->first).second, this->tables[i][index][j].second);
                    if (handle_conflicts(centroids, this->tables[i][index][j], i2c, temp_dist)) //If the manhattan distance to curent centroid is smaller then the previous(or if it is the first time we check current image)
                    {
                        (ti->second).push_back(this->tables[i][index][j]); //Insert image to current cluster
                    }
                }
            }
        }
    }

    for (auto ti = i2c.begin(); ti != i2c.end(); ++ti) //For every centroid in the vector, we start by checking at the same buckets
    {
        for (size_t q = 0; q < images.size(); q++)
        {
            for (size_t i = 0; i < this->L; i++)
            {

                int index = (unsigned int)this->g_functions[i]->apply(images[q].second) % this->table_size;
                if (c_buckets.find(index) == c_buckets.end())
                {
                    for (size_t j = 0; j < this->tables[i][index].size(); j++)
                    {
                        if (this->tables[i][index][j].first != (ti->first).first)
                        {
                            int temp_dist = manhattan_distance((ti->first).second, this->tables[i][index][j].second);
                            if (handle_conflicts(centroids, this->tables[i][index][j], i2c, temp_dist)) //If the manhattan distance to curent centroid is smaller then the previous(or if it is the first time we check current image)
                            {
                                (ti->second).push_back(this->tables[i][index][j]); //Insert image to current cluster
                            }
                        }
                    }
                }
            }
        }
    }
    
    return i2c;
}

template class LSH<int>;
template class LSH<double>;
