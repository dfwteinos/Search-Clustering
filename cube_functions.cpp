#include "hash_tables.h"

template <class T>
HyperCube<T>::HyperCube(vector_list_collection<T> t, int k, int L, int R, int M, int probes)
    : HashTable<T>(t, k, L, R), M(M), probes(probes)
{

    //Creating f_functions
    for (ssize_t i = 0; i < k; i++)
    {
        f_functions.push_back(new f_Function<T>(790, k));
        //  f_functions.push_back(h_functions);
    }

    fill_table(t);
}

template <class T>
void HyperCube<T>::fill_table(vector_list_collection<T> data_set)
{
    std::cout << "TEMP " << this->K << std::endl;
    std::cout << "in hypercube's fill table " << std::endl;
    T index;
    for (size_t q = 0; q < data_set.size(); q++)
    {
        std::string temp;
        for (size_t i = 0; i < this->K; i++)
        {
            temp += this->f_functions[i]->apply(data_set[q].second);
        }
        if (points.find(temp) != points.end())
        {
            points[temp].push_back(data_set[q]);
        }
        else
        {
            vector_list_collection<T> p;
            p.push_back(data_set[q]);
            points.insert({temp, p});
            point_values.insert(temp);
        }
        std::cout << "TEMP " << temp << std::endl;
    }
    std::cout << "exiting cube's fill table" << std::endl;
}

template <class T>
image<T> HyperCube<T>::approximateNN(image<T> query, T &res)
{
    image<T> best;
    int distance;

    if (this->probes > this->points.size())
        this->probes = this->points.size();
    int best_distance = std::numeric_limits<int>::max();
    std::string temp;

    int M_value = this->M;

    for (size_t i = 0; i < this->K; i++)
    {
        temp += this->f_functions[i]->apply(query.second);
    }

    std::cout << "query str" << temp << std::endl;

    int images_checked = 0;
    //check if there are other points with the same key

    // std::cout << "size " << points[temp].size() << std::endl;
    if (points.find(temp) != points.end())
    {
        for (auto it = points[temp].begin(); it < points[temp].end(); ++it)
        {
            distance = manhattan_distance<T>(query.second, it->second);
            std::cout << "DISTANCE " << distance << std::endl;
            if (distance <= this->R)
            {
                if (distance < best_distance)
                {
                    best = (*it);
                    best_distance = distance;
                }

                this->neighbours.insert((*it));
                images_checked++;
            }

            if (images_checked >= M_value)
            {
                M_value = 0;
                break;
            }
        }
        M_value = M_value - images_checked;
    }

    //check nearest points
    if (M_value > 0)
    {
        std::cout << "*********YEEEEEEEEEESSSSSS***********" << std::endl;
        int probes_value = this->probes;
        int points_checked = 0;
        //get nearest points
        std::map<std::string, int> nearest_points;
        for (auto it = point_values.begin(); it != point_values.end(); ++it)
        {
            nearest_points.insert({(std::string)(*it), hammingDist(temp, (std::string)(*it))});
        }
        std::cout << "nearest p " << nearest_points.size() << std::endl;
        //for all the nearest points find those within radius and calculate manhattan
        for (auto ti = nearest_points.begin(); ti != nearest_points.end(); ti++)
        {
            for (auto it = points[ti->first].begin(); it < points[ti->first].end(); ++it)
            {
                distance = manhattan_distance<T>(query.second, it->second);
                std::cout << "DISTANCE 2  " << distance << std::endl;
                if (distance <= this->R)
                {
                    if (distance < best_distance)
                    {
                        best = (*it);
                        best_distance = distance;
                    }

                    this->neighbours.insert((*it));
                    images_checked++;
                }

                if (images_checked >= M_value)
                {
                    M_value = 0;
                    break;
                }
            }
            --probes_value;
            if (probes_value <= 0)
            {
                break;
            }
        }
    }

    res = best_distance;
    return best;
}

template class HyperCube<int>;
template class HyperCube<double>;