#include "arguments.h"
#include "util.h"
#include <ctime>

int main(int argc, char **argv)
{
    std::string operation = get_operation(argv[0]);

    std::string input_file, query_file, output_file;

    int k = 0, L = 5, M = 10, N = 3, probes = 2;

    double R = 10000.0, c=2;

    if (get_parameters(argc, argv,
                       operation,
                       input_file,
                       query_file,
                       output_file,
                       k, L, M, N, probes,
                       R) < 3)
    {
        printf("Please check the recommended input options\n");
        return -1;
    }

    std::cout << operation << std::endl
              << input_file << std::endl;
    // std:: cout << k << std::endl;

   // read_file(input_file);

    bool repeat = true;

    while(repeat)
    {
        if (operation.compare("lsh") == 0)
            lsh<int>(input_file, query_file, k, L, R, N, c, output_file);
        else if (operation.compare("cube") == 0)
            cube<int>(input_file, query_file, k, M, R, N, c, probes, output_file);
       
        std::cout << "Do you want to continue?" << std::endl;
        
        std::string reply;
        
        std::cin >> reply;

        if (!reply.compare("no"))
            repeat = false;

        if (repeat == true)
        {

            std::cout << "input_file: ";
            std::cin >> input_file;
            std::cout << std::endl;
            std::cout << "query_file: ";
            std::cin >> query_file;
            std::cout << std::endl;
            std::cout << "output_file: ";
            std::cin >> output_file;
            std::cout << std::endl;
        }
    }
}