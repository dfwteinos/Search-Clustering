#include "hash_tables.h"
#include "arguments.h"


template <class T>
HashTable<T>::HashTable(id_vector_collection<T> data_set, int k_value, int l_value, int R_value){
    table_size = data_set.size()/8;
    vector_size = data_set[0].second.size();
    K = k_value;
    L = l_value;
    R = R_value;
    //Creating K h_functions
    for (ssize_t i = 0; i < K; i++){
        h_functions.push_back(new HFunction<T>(vector_size, K));
    }

    //Creating L g_functions
    for (ssize_t i = 0; i < L; i++){
        g_functions.push_back(new GFunction<T>(h_functions));
    }

    //Creatind L hash_tables
    tables.resize(L);
    for (size_t i = 0; i < L; i++){
        tables[i].resize(table_size);
    }
}

template <class T>
HashTable<T>::~HashTable(){
    for (ssize_t i = 0; i < K; i++)
         delete h_functions[i];
    for (ssize_t i = 0; i < L; i++)
         delete g_functions[i];
}

template <class T>
LSH<T>::LSH(id_vector_collection<T> data_set, int k_value, int l_value, int R)
        :HashTable<T>(data_set, k_value, l_value, R){
           fill_table(data_set); 
        }

template <class T>
void LSH<T>::fill_table(id_vector_collection<T> data_set){
    std::cout << "in fill table " << std::endl;
    int index;
    for (size_t q = 0; q < data_set.size(); q++){    
        for (size_t i = 0; i < this->L; i++){
            index = this->g_functions[i]->apply(data_set[q].second)% this->table_size;
            this->tables[i][index].push_back(data_set[q]);
        }    
    }  
}

template <class T>
void HashTable<T>::table_state(){
    for (size_t i = 0; i < tables.size() - 1; i++) {
        std::cout << "Table " << i << std::endl;
        for (size_t j = 0; j < tables[i].size(); j++) {
            std::cout << "\tContainer " << j << " size " << tables[i][j].size() << std::endl;
        }        
    }
}

template <class T> 
T manhattan_distance(std::vector<T> x, std::vector<T> y){
    T result = 0;
    for (size_t i = 0; i < x.size(); i++) {
        result += abs(x[i] - y[i]);
    }
    return result;
}


template <class T>
id_vector<T> LSH<T>::approximateNN(id_vector<T> query, T &res){
     id_vector<T> best;
     T best_distance = std::numeric_limits<T>::max();
     for (size_t i = 0; i < this->L; i++) {
         int full_index = this->g_functions[i]->apply(query.second);
         int index = full_index%this->table_size;
         int count = 0;
         for (size_t j = 0; j < this->tables[i][index].size(); j++) {
            int data_index = this->g_functions[i]->apply(this->tables[i][index][j].second);
            if (data_index == full_index) count++;
            if (count > 10*this->L){ 
                break;
            }
             T distance = manhattan_distance<T>(query.second, this->tables[i][index][j].second);
             if (distance < best_distance) {
                best = this->tables[i][index][j];
                best_distance = distance;
             }
             if(distance <= this->R){
                 this->neighbours.insert(this->tables[i][index][j]);
             }
         }
     }
    res = best_distance;
     
    return best;
 }


template <class T>
id_vector<T> bruteforce(id_vector_collection<T> data_set, id_vector<T> query, T &res){
    id_vector<T> best;
    T best_distance = std::numeric_limits<T>::max();
    T current;
    for(ssize_t i = 0; i < data_set.size(); i++) {
        current = manhattan_distance<T>(data_set[i].second, query.second);
        if (current < best_distance) {
            best_distance = current;
            best = data_set[i];
        }     
    }
    res = best_distance;
    
    return best;
}


template <class T>
std::pair<double,id_vector_collection<T>> HashTable<T>::vectorise_data(std::string file){
    std::pair<double,id_vector_collection<T>> vectors;
    vectors.first = 0;
    std::string line, tmp;
    
    std::ifstream inputFile;         //initiate reading input file
    inputFile.open(file);
    if(inputFile.is_open()){
        while(getline(inputFile, line)){
            std::stringstream line_strm(line);
            std::vector<T> current;
            std::string id;
            line_strm >> tmp;
            id = tmp;
            if(id.compare("Radius:")==0){
                //std::cout << "*******ya******" << std::endl;
                line_strm >> tmp;
                vectors.first = stod(tmp);
                //std::cout << vectors.first << std::endl;
            }
            else{
            while(line_strm >> tmp){    
                current.push_back(stoi(tmp));       //each vector of element
            }
            vectors.second.push_back(std::pair<std::string, std::vector<T> >(id,current));             //all elements
        }
        }
    }
    inputFile.close();
    return vectors;
}

template <class T>
void HashTable<T>::get_neighbours(std::ostream& my_file){
    if(this->R != 0){
        for(auto it = this->neighbours.begin(); it != this->neighbours.end(); it++){
            my_file << (*it).first << std::endl;
        }
    }
    this->neighbours.clear();
    return;
}

template <class T>
HyperCube<T>::HyperCube(id_vector_collection<T> t, int k, int L,int R, int M, int probes)
    : HashTable<T>(t, k, L, R), M(M), probes(probes) { fill_table(t); }


template <class T>
void HyperCube<T>::fill_table(id_vector_collection<T> data_set){
    std::cout << "in hypercube's fill table " << std::endl;
    T index;
    for (size_t q = 0; q < data_set.size(); q++){
        std::string temp;
        for (size_t i = 0; i < this->L; i++){
            index = this->g_functions[i]->apply(data_set[q].second);
            if (dict.find(index) == dict.end()){
                this->dict.insert(std::make_pair(index, std::to_string((rand() > RAND_MAX / 2) ? 0 : 1)));
            }
            temp += dict[index];
        }
        f.insert(make_pair(temp, data_set[q]));
    }
}


template <class T>
id_vector<T> HyperCube<T>::approximateNN(id_vector<T> query, T &res){
    id_vector<T> best;
    if(this->probes > this->f.size()) this->probes = this->f.size();
    int best_distance = std::numeric_limits<int>::max();
    std::string temp;
    for (size_t i = 0; i < this->L; i++){
        int index = this->g_functions[i]->apply(query.second) % this->table_size;
        if (dict.find(index) == dict.end()){
                this->dict.insert(std::make_pair(index, std::to_string((rand() > RAND_MAX / 2) ? 0 : 1)));
            }
        temp += dict[index];
    }
    int count = 0, diff= 0;
    while((count < this->probes)){
        int q = 0;
        for (auto it = f.begin(); it != f.end(); ++it){
            if (hammingDist(temp, it->first,diff)){ 
                count++;
                q = 0;
                auto curr = f.equal_range(it->first);
                for(auto ti = curr.first; ti != curr.second; ++ti){
                    q++;
                    int distance = manhattan_distance<T>(query.second, ti->second.second);
                    if (distance < best_distance){
                        best = ti->second;
                        best_distance = distance;
                    }
                    if(distance <= this->R){
                        this->neighbours.insert(ti->second);
                    }
                    if(q >= this->M) break;
                }
                if(q >= this->M) break;
            }
            if((q >= this->M) || (count >= this->probes)) break;
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
template id_vector<int> bruteforce(id_vector_collection<int> , id_vector<int>, int&);
template id_vector<double> bruteforce(id_vector_collection<double> , id_vector<double>, double&);

