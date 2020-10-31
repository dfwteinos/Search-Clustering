#include "hash_functions.h"

bool hammingDist(std::string str1, std::string str2, int diff){
    int i = 0, count = 0;
    while (str1[i] != '\0'){
        if (str1[i] != str2[i])
            count++;
        i++;
    }
    return(count == diff);
}


int mult_power(unsigned int base, int exp, int mod) {               // m , 784 - i - 1 , M
    int res = 1; 
    base = base % mod; 
    while (exp > 0) { 
        if (exp & 1) res = (res*base) % mod; 
  
        exp = exp>>1; 
        base = (base*base) % mod;   
    } 
    return res; 
}


inline int modulo(int number, int mod_number){
    int mult = 0;
    if (number > 0) return number % mod_number;
    while (number + mult*mod_number <= 0)
        mult++;
    return (number + mult*mod_number) % mod_number;
}

unsigned int mod(int a, int m ){

    if( a % m < 0){
        return (unsigned int) a % m + m;
    }
    
    else {
        return (unsigned int) a % m;
    }
}

template <class T> 
// int HFunction<T>::apply(std::list<T> q){
    int HFunction<T>::apply(std::vector<T> q){                      //TODO

    unsigned int result = 0;
    int size = q.size();
    int part_a;
    int part_b;

    std::list<T> test;

    // std::list<T> q_temp = q;
    std::vector<T> q_temp = q;                                              //TODO

    for (size_t i = 0; i < size; i++) {

        // int a = std::floor((q_temp.front() - s[i])/w);                      //why front(?)..
        int a = std::floor((q_temp[i] - s[i])/w);                      //why front(?)..

        // result += modulo(a, M) * m_power[i];                             //According to slides it should be: modulo(a*m_power[i],M)
        // result = modulo(a)
        // part = a*m_power[i];
        // result += modulo(part,M);

        //I want to have: (a[]b) mod M = ((a mod M)[](b mod M)) mod M

        part_a = mod(a,M);
        part_b = mod(m_power[i],M);
        result += mod( (part_a * part_b), M );
        test.push_back(s[i]);
    
    }
    // return modulo(result, M);
    // for (auto v: test)
        // std::cout << v << std::endl;

    return result;
    
    
}



template <class T>
GFunction<T>::GFunction(std::vector< HFunction< T >* > h_functions){
    std::cout << "In g function" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_int(0, h_functions.size()-1);
    
    std::cout << h_functions.size() << std::endl;
    for (size_t i = 0; i < h_functions.size(); i++) {
        functions.push_back(h_functions[random_int(gen)]);                      //May concat: g(x) = [h_2(x)|h_1(x)] or [h_1(x)|h_2(x)]
    }
}


template < class T>
// int GFunction<T>::apply(std::list<T> q){                                     //TODO
int GFunction<T>::apply(std::vector<T> q){

    int result = 0;
    for (size_t i = 0; i < functions.size(); i++){
        int temp = functions[i]->apply(q);                                          //Computes each hi(x), until hk(x)

        // result = result ^ temp;                                                  //These 2 lines are same as:            
        result <<= 8;                                                               //result = result << 4 | temp (?)
        result |= temp;
    }  
    return result;
}

template <class T>
void HFunction<T>::vector_init(std::vector<double> &v, int size) {
    std::cout << "Initiate Vector values: ";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> random_real(0, w);

    for (ssize_t i = 0; i < size; i++) {
        v.push_back(random_real(gen));
        std::cout << v[i] << " ";
    }

}

template <class T>
HFunction<T>::HFunction(int size, int K) {

    w = 4000;                                                                               //Should be l*avg.Distance(l is an positive whole number)(?)
    M = (int) pow(2,(32/K));
    // m = (unsigned int) pow(2, 32) - 4;                                                    
    m = (unsigned int) pow(2,29) - 5;                                                       //According to new slides
    for(ssize_t i = 0; i < size; i++) {
        m_power.push_back(mult_power(m, size - i - 1, M));                                  //Computes: each m, m^2, m^3, ... , m^(d-1)
    }
    vector_init(s, size);                                                                   //Computes each: { s_0 , s_1, ... , s_(d-1) }
}

template class HFunction<int>;
template class GFunction<int>;
template class HFunction<double>;
template class GFunction<double>;