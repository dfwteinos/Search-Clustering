#include "hash_functions.h"

bool hammingDist(std::string str1, std::string str2)
{
    // int i = 0, count = 0;
    // while (str1[i] != '\0'){
    //     if (str1[i] != str2[i])
    //         count++;
    //     i++;
    // }
    // return(count == diff);
    int i = 0, count = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
            count++;
        i++;
    }
    return count;
}

int mult_power(unsigned int base, int exp, int mod)
{
    int res = 1;
    base = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
            res = (res * base) % mod;

        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

inline int modulo(int number, int mod_number)
{
    int mult = 0;
    if (number > 0)
        return number % mod_number;
    while (number + mult * mod_number <= 0)
        mult++;
    return (number + mult * mod_number) % mod_number;
}

template <class T>
int HFunction<T>::apply(std::list<T> q)
{
    int result = 0;
    int size = q.size();
    std::list<T> q_temp = q;
    auto it = q.begin();
    for (size_t i = 0; i < size; i++)
    {
        int a = std::floor(((*it) - s[i]) / w);
        result += modulo(a, M) * m_power[i];
        ++it;
    }
    return result%M;//modulo(result, M);
}

template <class T>
GFunction<T>::GFunction(std::vector<HFunction<T> *> h_functions)
{
    std::cout << "in g function" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_int(0, h_functions.size() - 1);

    for (size_t i = 0; i < h_functions.size(); i++)
    {
        functions.push_back(h_functions[random_int(gen)]);
    }
}

template <class T>
int GFunction<T>::apply(std::list<T> q)
{
    int result = 0;
    for (size_t i = 0; i < functions.size(); i++)
    {
        int temp = functions[i]->apply(q);
        result = result ^ temp;
        result <<= 4;
    }
    return result;
}

template <class T>
void HFunction<T>::vector_init(std::vector<double> &v, int size)
{
    std::cout << "Initiate Vector values: ";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> random_real(0, w);

    for (ssize_t i = 0; i < size; i++)
    {
        v.push_back(random_real(gen));
        // std::cout << v[i] << " ";
    }
}

template <class T>
HFunction<T>::HFunction(int size, int K)
{
    w = 6000;
    M = (int)pow(2, (32 / K));
    m = (unsigned int)pow(2, 32) - 4;
    for (ssize_t i = 0; i < size; i++)
    {
        m_power.push_back(mult_power(m, size - i - 1, M));
    }
    vector_init(s, size);
}

template <class T>
f_Function<T>::f_Function(int vector_size, int k) : generator((std::random_device())())
{
    this->hf = new HFunction<T>(vector_size, k);
}

// template <class T>
// f_Function<T>::f_Function(std::vector<HFunction<T> *>) : generator((std::random_device())())
// {
//     for (auto i = h_functions.begin(); i < h_functions.end(); ++i)
//     {
//         int num = (*i)->apply(q);
//         int random;
//         std::uniform_int_distribution<> dis(0, 1);
//         random = dis(generator);
//         // std::cout << num << std::endl;
//         if (this->dict.find(num) == this->dict.end())
//         {

//             this->dict.insert(std::make_pair(num, std::to_string(random)));
//         }
//     }
// }

template <class T>
std::string f_Function<T>::apply(std::list<T> q)
{

    int num = hf->apply(q);
    int random;
    std::uniform_int_distribution<> dis(0, 1);
    random = dis(generator);
    // std::cout << num << std::endl;
    if (this->dict.find(num) == this->dict.end())
    {

        this->dict.insert(std::make_pair(num, std::to_string(random)));
    }
    return dict[num];

     
    //     int random;
    //     std::uniform_int_distribution<> dis(0, 1);
    //     random = dis(generator);
    //     std::string temp;
    // for (auto i = h_functions.begin(); i < h_functions.end(); ++i)
    // {
    //    int num = (*i)->apply(q);
    //     // std::cout << num << std::endl;
    //     if (this->dict.find(num) == this->dict.end())
    //     {

    //         this->dict.insert(std::make_pair(num, std::to_string(random)));
    //     }
    //     temp += dict[num]
    // }
    // return temp;
}

template class HFunction<int>;
template class GFunction<int>;
template class f_Function<int>;
template class HFunction<double>;
template class GFunction<double>;
template class f_Function<double>;