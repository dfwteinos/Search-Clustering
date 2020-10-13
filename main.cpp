#include "arguments.h"
#include "util.h"
#include <ctime>

int main(int argc, char **argv)
{
    std::string operation = get_operation(argv[0]);

    std::string input_file, query_file, output_file;

    int k = 0, L = 5, M = 10, N = 1, probes = 2;

    double R = 1.0;

    if (get_parameters(argc, argv, 
                        operation,
                        input_file, 
                        query_file, 
                        output_file, 
                        k, L, M, N, probes,
                         R) < 3) {
        printf("Please check the recommended input options\n");
        return -1;
    }
   
    std:: cout << operation << std::endl << input_file << std::endl;
    // std:: cout << k << std::endl;

    read_file(input_file);
}