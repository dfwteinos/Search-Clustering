#include "arguments.h"

std::string get_operation(char *name)
{
    std::string operation = name;
    std::size_t index = operation.find_last_of("/");
    operation = operation.substr(index + 1);
    return operation;
}

int get_parametres(int argc, char **argv,
                   std::string &input_file,
                   std::string &query_file,
                   std::string &output_file,
                   int &k, int &L, int &M, int &N, int &probes,
                   double &R)
{
    int check = 0;
    for (int i = 1; i < argc; i += 2)
    {
        std::string temp = argv[i];
        
        if (!strcmp(argv[i], "-d"))
        {
            input_file = argv[i + 1];
            check++;
        }
        else if (!strcmp(argv[i], "-q"))
        {
            query_file = argv[i + 1];
            check++;
        }
        else if (!strcmp(argv[i], "-k"))
        {
            k = std::stoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-L"))
        {
            L = std::stoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-o"))
        {
            output_file = argv[i + 1];
            check++;
        }
        else if (!strcmp(argv[i], "-M"))
        {
            M = std::stoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-N"))
        {
            N = std::stoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-R"))
        {
            R = std::stoi(argv[i + 1]);
            check++;
        }
        else if (!strcmp(argv[i], "-probes"))
        {
            probes = std::stoi(argv[i + 1]);
        }
    }
    return check;
}
