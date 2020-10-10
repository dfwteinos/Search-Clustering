#include "arguments.h"
#include <string>
#include <iostream>
#include <ctime>

int main(int argc, char **argv)
{
    std::string operation = get_operation(argv[0]);
    std::string input_file, query_file, output_file;
    int k = 4, L = 5, M = 10, N = 1, probes = 2;
    double R = 1.0;

    if (get_parametres(argc, argv, 
                        input_file, 
                        query_file, 
                        output_file, 
                        k, L, M, N, probes,
                         R) < 3) {
        printf("Please check the recommended input options\n");
        return -1;
    }
   
   std:: cout << operation << std::endl;
}