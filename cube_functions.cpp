#include "hash_tables.h"

template <class T>
HyperCube<T>::HyperCube(vector_list_collection<T> data_set, int k_value, int l_value, int R_value, int N_value, int M_value, double C_value, int probes_value)
    : HashTable<T>(data_set, k_value, l_value, R_value, N_value, C_value), M(M_value), probes(probes_value)
{

    //Creating f_functions
    for (ssize_t i = 0; i < this->K; i++)
    {
        f_functions.push_back(new f_Function<T>(this->vector_size, this->K));
        //  f_functions.push_back(h_functions);
    }

    fill_table(data_set);
}

template <class T>
HyperCube<T>::~HyperCube()
{
    for (ssize_t i = 0; i < this->K; i++)
    {
        delete this->f_functions[i];
    }
}

template <class T>
void HyperCube<T>::fill_table(vector_list_collection<T> data_set)
{
    std::cout << "In Fill Table " << std::endl;
    T index;
    for (size_t q = 0; q < data_set.size(); q++)
    {
        std::string temp;
        for (size_t i = 0; i < this->K; i++)
        {
            temp += this->f_functions[i]->apply(data_set[q].second);
        }
        if (points.find(temp) != points.end()) //If there already is a point with the same indetifier
        {
            points[temp].push_back(data_set[q]); //Add the image to the existing vector list of that point
        }
        else //If it is the first point with the calculated identifier
        {
            vector_list_collection<T> p; //Add that image to a new vector list of that point
            p.push_back(data_set[q]);
            points.insert({temp, p});
        }
        std::cout << "TEMP " << temp << std::endl;
    }
    std::cout << "Exiting Fill Table" << std::endl;
    return;
}

template <class T>
std::vector<cand_img<T>> HyperCube<T>::aNNeighbours(image<T> query, int N, std::vector<cand_img<T>> &RS_vector)
{ //Returns K-Nearest Neighbours and their Distance from point q.

    std::vector<cand_img<T>> best_imgs; //Vector with pair <:Image , Distance>                                                       //Vector with pair <:Image , Distance>
    std::vector<cand_img<T>> RS_imgs;   //Vector with all the images that are inside our range search

    RS_vector.clear();
    best_imgs.clear();

    image<T> temp_img; //Each image that we check                                                                              //Each image that we check

    T temp_dist; //Each distance that we check                                                                                 //Each distance that we check

    cand_img<T> temp_cand;

    if (this->probes > this->points.size())
        this->probes = this->points.size();
    int best_distance = std::numeric_limits<int>::max();
    std::string temp_point;

    for (size_t i = 0; i < this->K; i++) //Get the query's position(point identifier) in the hypercube
    {
        temp_point += this->f_functions[i]->apply(query.second);
    }
    std::cout << "query str" << temp_point << std::endl;

    int images_checked = this->M;

    //Search nearest points
    if (images_checked > 0 && this->probes > 0)
    {
        int points_checked = this->probes;
        //Get nearest points
        std::map<int, std::string> nearest_points;
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            nearest_points.insert({hammingDist(temp_point, (std::string)(it->first)), (std::string)(it->first)}); //For every point in the cube find the Hamminf distance between that and query's point
        }

        //For all the nearest points according to the Hamming distance we calculated
        for (auto ti = nearest_points.begin(); ti != nearest_points.end(); ++ti)
        {
            for (auto it = points[ti->second].begin(); it != points[ti->second].end(); ++it) //For every image in currenmt point
            {
                temp_img = (*it);
                temp_dist = manhattan_distance<T>(query.second, it->second); //Each calculated distance between query and image
                std::cout << "DISTANCE 2  " << temp_dist << std::endl;
                temp_cand = make_pair(temp_img, temp_dist);

                if (temp_dist <= (unsigned int)((this->R) * (this->c)))
                { //If the distance is inside the (R-Ball) then push back this image
                    RS_imgs.push_back(temp_cand);
                    best_imgs.push_back(temp_cand);
                    images_checked--;
                }

                if (images_checked <= 0)
                {
                    std::sort(best_imgs.begin(), best_imgs.end(), sortbysec<T>);

                    break;
                }
            }

            if (images_checked <= 0)
                break;

            --points_checked;
            if (points_checked <= 0) //If we have searched #probes points
            {
                if (best_imgs.size() != 0) //And found at least one in given R-range, calculate best neighboors
                {
                    std::sort(best_imgs.begin(), best_imgs.end(), sortbysec<T>);
                }
                break;
            }
        }
    }

    if (best_imgs.size() > N) //In case M > N, return the N best neighboors
    {
        for (int i = 0; i < (best_imgs.size() - N); i++)
        {
            auto last = best_imgs.end();
            best_imgs.erase(last);
        }
    }

    RS_vector = RS_imgs;
    return best_imgs;
}

template <class T>
clusters<T> HyperCube<T>::reverse_assignment(vector_list_collection<T> &centroids)
{

    clusters<T> i2c;
    std::map<std::string, vector_list_collection<T>> c_points; //To store centroids based on their point in the cube

    for (auto ti = centroids.begin(); ti != centroids.end(); ++ti) //Initialize vector of centroids
    {
        vector_list_collection<T> v;
        i2c.push_back(std::make_pair((*ti), v));
        std::string temp_point;

        for (size_t i = 0; i < this->K; i++) //Get the centroid's position(point identifier) in the hypercube
        {
            temp_point += this->f_functions[i]->apply(ti->second);
        }

        std::cout << "centroid :" << temp_point << std::endl;
         if (c_points.find(temp_point) == c_points.end()) //Keep centroid's position in map
        {
            vector_list_collection<T> temp;
            temp.push_back((*ti));
            c_points.insert({temp_point, temp});
        }
        else
        {
            c_points[temp_point].push_back((*ti));
        }
    }

    //Manage the collisions first
    for (auto p = points.begin(); p != points.end(); ++p)
    {

        for (auto ti = i2c.begin(); ti != i2c.end(); ++ti) //For every centroid in the vector, we start by checking at the same points
        {

            if (c_points.find(p->first) != c_points.end())
            {
                for (auto it = points[p->first].begin(); it != points[p->first].end(); ++it) //Find if any of the images already exists in a cluster
                {
                    if (it->first != (ti->first).first)
                    {
                        int temp_dist = manhattan_distance((ti->first).second, it->second);
                        if (handle_conflicts(centroids, (*it), i2c, temp_dist) == true) //If the manhattan distance to curent centroid is smaller then the previous(or if it is the first time we check current image)
                        {
                            std::cout << "image " << (it->first) << " added to cluster " << (ti->first).first << " !" << std::endl;
                            (ti->second).push_back((*it)); //Insert image to current cluster
                        }
                    }
                }
            }
            else
            {
                std::cout << "unassigned point" << std::endl;
                for (auto it = (p->second).begin(); it != (p->second).end(); ++it) //Find if any of the images already exists in a cluster
                {
                    if (it->first != (ti->first).first)
                    {
                        int temp_dist = manhattan_distance((ti->first).second, it->second);
                        if (handle_conflicts(centroids, (*it), i2c, temp_dist) == true) //If the manhattan distance to curent centroid is smaller then the previous(or if it is the first time we check current image)
                        {
                            std::cout << "image " << (it->first) << " added to cluster " << (ti->first).first << " !" << std::endl;
                            (ti->second).push_back((*it)); //Insert image to current cluster
                        }
                    }
                }
            }
        }
    }

    return i2c;
}

template class HyperCube<int>;
template class HyperCube<double>;