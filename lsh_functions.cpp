#include "hash_tables.h"
#include "arguments.h"

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
            {
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
    }
    res = best_distance;

    return best;
}

template class LSH<int>;
template class LSH<double>;
