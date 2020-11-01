#include "hash_tables.h"
#include "util.h"
// #include "arguments.h"

template <class T>
void k_means(vector_list_collection<T> input , vector_list_collection<T> &centroids, int kClusters){

    int cur_centroid;                                                                       // Each centroid's index
    int ccounter=0;                                                                         // Centroids Counter
    int max_dist = 0; 
    double x;
    double max_prob = 0.0;

    image<T> each_centroid;                                                                 // Each centroid's image
    distances<T> distances;                                                                 // All D(i), where i = {1, 2, . . . , n-t}

    std::vector<double> probs;                                                              // All P(r), where r = {1, 2, . . . , n-t}
    std::vector<double> normal_distances;                                                   // All D(i)/max_D(i)

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0,input.size());                        // From 0 up to 59000 .

    cur_centroid = distribution(gen);                                                       // First centroid will be choosen randomly
    
    centroids.push_back(input[cur_centroid]);                                              
    each_centroid = input[cur_centroid];

    input.erase(input.begin() + cur_centroid);                                              // Take out this datapoint from dataset
    ccounter++;

    for(int k = 1; k<kClusters; k++){                                                       // Find all centroids

        find_nearest_centroid(input, distances, each_centroid, k, max_dist);   
        normalization_Distances(distances, normal_distances, max_dist);                     
        compute_probs(normal_distances, probs);
        choose_random_x(probs,x);

        cur_centroid = upper_bound(probs.begin(), probs.end(), x) - probs.begin();              // Computing the next centroid's index from data input

        centroids.push_back(input[cur_centroid]);                                              
        each_centroid = input[cur_centroid];
        input.erase(input.begin() + cur_centroid);                                              // Take out this datapoint from dataset
        distances.erase(distances.begin() + cur_centroid);                                      // Take out this datapoint from dataset

        ccounter++;
        normal_distances.clear();
        probs.clear();
    }

    return;
}

template <class T>
void return_index(std::vector<T> datapoint, std::vector<cluster<T>> clusters, int &best_next_centroid,int current){

    int temp_dist;
    int best_dist = INT_MAX;

    for(int i = 0 ; i < clusters.size(); i++){
        if(current!=i)
        {
            temp_dist = manhattan_distance( clusters[i].first.second, datapoint );
            if( temp_dist < best_dist)
            {
                best_dist = temp_dist;
                best_next_centroid = i;
            }
        }
    }
}


template <class T>
void Silhouette(std::vector<cluster<T>> clusters, std::vector<double>& s_vector){

    std::vector<unsigned int> a, b, s, s_clusters;
    unsigned int a_i, b_i, s_i;
    int best_next_centroid;                                                 

    for(int k = 0; k < clusters.size(); k++){                                           //For each cluster
        
        a.clear();
        b.clear();

        std::cout << k << std::endl;
        for(int s = 0; s < clusters[k].second.size(); s++){                             //For all the images in this cluster

            a_i = 0; 
            b_i = 0;

            return_index(clusters[k].second[s].second , clusters, best_next_centroid, k);

            for(int t = 0; t < clusters[k].second.size(); t++){                         //For each image, check all its neighbour

                if(s!=t) a_i += a_i + manhattan_distance( clusters[k].second[s].second, clusters[k].second[t].second );
            }

            for(int l = 0; l < clusters[best_next_centroid].second.size(); l++)
            {

                b_i += b_i + manhattan_distance( clusters[best_next_centroid].second[l].second, clusters[k].second[s].second );
            }

            a.push_back(a_i/clusters[k].second.size());
            b.push_back(b_i/clusters[best_next_centroid].second.size());

        }

        unsigned int avg=0;
        double max;
        for(int i =0 ; i < a.size(); i++){
        
            max = (a[i] > b[i]) ? a[i] : b[i] ; 
            s.push_back( (b[i] - a[i]) / max );
            avg += avg + s[i];
        }

        s_clusters.push_back(avg/s.size());

    }
}


template <class T>
double checkVariation(vector_list_collection<T> centroids,vector_list_collection<T> next_centroids){

    double sum_variation=0;
    if(centroids.size()==0 || next_centroids.size()==0) return -1.0;                           

    for(int i = 0; i < centroids.size(); i++ ){
        
        sum_variation+= manhattan_distance( centroids[i].second, next_centroids[i].second );
        std::cout << sum_variation << std::endl;
    }

    return sum_variation;
}


template <class T>
void k_Medians(std::vector<cluster<T>> clusters, vector_list_collection<T>& next_centroids, int pixels){

    std::vector<std::vector<T>> median_centroid(pixels);
    
    for(int k = 0; k < clusters.size(); k++){                                   //For each cluster
        for(int s = 0; s<clusters[k].second.size(); s++){                       //For all the data points|< <img>,...,<imgs> >|who belongs to this cluster                  
            for(int p = 0; p<clusters[k].second[s].second.size(); p++){         //For 784 pixels for each img in this specific cluster

                median_centroid[p].push_back(clusters[k].second[s].second[p]);  
            }
        }
    
        image<T> newC;
        for(int i = 0; i < pixels; i++){
            
            newC.second.push_back(computeMedian(median_centroid[i]));
        }

        newC.first = k;
        next_centroids.push_back(newC);
    }

    return;
}

template <class T>
T computeMedian(std::vector<T> eachPixel){

    size_t size = eachPixel.size();

    if (size == 0 )
    {
        return 0;       //Undefined
    }

    else {

        sort(eachPixel.begin(), eachPixel.end());
        if ((size%2) == 0)
        {
            return (eachPixel[size/2]);
        }
        else 
        {
            return (eachPixel[ceil(size/2)]);

        }
    }
}

template <class T>
void Lloyds(vector_list_collection<T> input, std::vector<cluster<T>>& clusters, vector_list_collection<T> centroids){
    

    for(int j = 0; j < centroids.size(); j++){                                             // Initialize clusters

        cluster<T> each_cluster;
        each_cluster.first = (centroids[j]);
        clusters.push_back(each_cluster);
    }

    int best_dist;
    int temp_dist;
    int best_index;                                                                         // Index for the best centroid among all, for each data point

    for (int i = 0; i < input.size(); i++){
        best_dist = INT_MAX;

        for( int k = 0; k < centroids.size(); k++){

            temp_dist = manhattan_distance( clusters[k].first.second, input[i].second );
            if ( temp_dist < best_dist ) {
                best_dist = temp_dist;
                best_index = k;
            }
        }

        clusters[best_index].second.push_back(input[i]);
    }

    return;
}


void choose_random_x(std::vector<double> probs, double &x){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double>  distribution(0, probs[probs.size() - 1]);                        // From 0 up to 1 .

    x = distribution(gen);
    std::cout << x << std::endl;
    return;
}

void compute_probs(std::vector<double> distances, std::vector<double>& probs){

    probs.push_back(0);                                                                     // P(0) = 0

    for(int i = 0 ; i < distances.size(); i++){
        probs.push_back( probs[i] + pow(distances[i],2));                                   // i = 0 but we start from prob[1]!
    }
}

template <class T>
void normalization_Distances(distances<T> best_distances, std::vector<double> &normal_distances, int max_dist){

    for (int i = 0 ; i < best_distances.size(); i++){
        normal_distances.push_back( (double) best_distances[i][0]/ (max_dist) );
    }
}

template <class T>
void find_nearest_centroid(vector_list_collection<T> input, distances<T>& best_distances, image<T> each_centroid, int ccounter, int& max_dist){

    std::vector<T> curr_dist ; 
    
    distances<T> temp_distances(input.size());
    // if(best_distances.size())  best_distances<T>(input.size());

    for (int i = 0; i<input.size(); i++){

        if (ccounter>1) curr_dist = best_distances[i];

        curr_dist.push_back ( manhattan_distance(each_centroid.second, input[i].second) );
        std::sort(curr_dist.begin(), curr_dist.end());
        
        if(ccounter>1) best_distances[i] = curr_dist;
        else best_distances.push_back(curr_dist);

        if( curr_dist.back() > max_dist ) max_dist = curr_dist.back();
        curr_dist.clear();
    }
}

template void k_means<int>(vector_list_collection<int>, vector_list_collection<int>&, int);
template void k_means<double>(vector_list_collection<double>, vector_list_collection<double>&, int);

template void find_nearest_centroid<int>(vector_list_collection<int>, distances<int>&, image<int>, int, int&);   
template void find_nearest_centroid<double>(vector_list_collection<double> ,distances<double>&, image<double>, int, int&);   

template void normalization_Distances<int>(distances<int> , std::vector<double>& , int);
template void normalization_Distances<double>(distances<double> , std::vector<double>&, int);

template void Lloyds<int>(vector_list_collection<int>, std::vector<cluster<int>>& , vector_list_collection<int>);
template void Lloyds<double>(vector_list_collection<double>, std::vector<cluster<double>> & , vector_list_collection<double>);

template void k_Medians<int>(std::vector<cluster<int>> , vector_list_collection<int>&, int);
template void k_Medians<double>(std::vector<cluster<double>>, vector_list_collection<double>&, int);

template int computeMedian<int>(std::vector<int>);
template double computeMedian<double>(std::vector<double>);

template double checkVariation<int>(vector_list_collection<int>, vector_list_collection<int>);
template double checkVariation<double>(vector_list_collection<double>, vector_list_collection<double>);

template void Silhouette<int>(std::vector<cluster<int>> clusters, std::vector<double>&);
template void Silhouette<double>(std::vector<cluster<double>> clusters, std::vector<double>&);

template void return_index<int>(std::vector<int>, std::vector<cluster<int>>, int&, int);
template void return_index<double>(std::vector<double>, std::vector<cluster<double>>, int&, int);
