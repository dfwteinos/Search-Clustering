#include "../include/arguments.h"

std::string get_operation(char *name)
{
    std::string operation = name;
    std::size_t index = operation.find_last_of("/");
    operation = operation.substr(index + 1);
    return operation;
}

int get_parameters(int argc, char **argv,
                    std::string operation,
                    std::string &input_file,
                    std::string &query_file,
                    std::string &output_file,
                    std::string &cluster_conf,
                    std::string &complete,
                    std::string &method,
                    int &k, int &L, int &M, int &N, int &probes, int &kClusters, int &kLSH, int &kCUBE,
                    double &R)
{
    int check = 0;

    if ( (operation.compare("lsh")==0) && (argc!=15 && argc!=7) ) return check;

    if ( (operation.compare("cube")==0) && (argc!=17 && argc!=7) ) return check;

    if ( (operation.compare("cluster")==0) && (argc!=11 && argc!=9) ) return check;

    set_default_parameter(operation,argc,k);

    for (int i = 1; i < argc; i += 2)
    {
        std::string temp = argv[i];
        
        if (!strcmp(argv[i], "-i"))
        { 
            input_file = argv[i + 1];
            check++;
        }
        if (!strcmp(argv[i], "-c"))
        {
            cluster_conf = argv[i + 1];
            check++;
        }
        if (!strcmp(argv[i], "-complete"))
        {
            complete = argv[i + 1];
            check++;
        }
        if (!strcmp(argv[i], "-m"))
        {
            method = argv[i + 1];
            check++;
        }
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

    if (!(operation.compare("cluster"))){ 

        if (get_cluster_parameters( cluster_conf,       
                                    method,
                                    kClusters, L, kLSH, M, kCUBE, probes
                                    ) < 6)
        {
            printf("Please check the recommended input options for clustering\n");
            return -1;
        }
    }

    return check;
}

int get_cluster_parameters( std::string cluster_conf,       
                            std::string &method,
                            int &kClusters, int &L, int &kLSH, int &M, int &kCUBE, int &probes)
{
    std::ifstream file;                                                 // Stream class to read from files
    std::string line;                                                   // Read each line of the file
    std::string delimiter = ":";                                        // To separate string and number
    std::string token;

    file.open(cluster_conf, std::ios::in);                              // in(open for input operations)       

    int num_of_args = 0;

    while( std::getline(file,line)){                                    // Reach line by line the file

        // std::cout << line.substr(line.find(" ") + 1) <<  std::endl;
        token = line.substr(0, line.find(delimiter));

        if(! token.compare("number_of_clusters"))
        {
            kClusters = std::stoi(line.substr(line.find(" ") + 1));
            num_of_args++;
        }
        if(! token.compare("number_of_vector_hash_tables"))
        {
            L = std::stoi(line.substr(line.find(" ") + 1));
            num_of_args++;
        }
        if(! token.compare("number_of_vector_hash_functions"))
        {
            kLSH = std::stoi(line.substr(line.find(" ") + 1));
            num_of_args++;
        }
        if(! token.compare("max_number_M_hypercube"))
        {
            M = std::stoi(line.substr(line.find(" ") + 1));
            num_of_args++;
        }
        if(! token.compare("number_of_hypercube_dimensions"))
        {
            kCUBE = std::stoi(line.substr(line.find(" ") + 1));
            num_of_args++;
        }
        if(! token.compare("number_of_probes"))
        {
            probes = std::stoi(line.substr(line.find(" ") + 1));
            num_of_args++;
        }
    }

    return num_of_args;
}


void set_default_parameter(std::string operation,                //set default parameters if user doesen't give them
                int argc,
                int &k)
{

    if ( (operation.compare("lsh")==0) && argc==7 ) {

        k=4;
    }

    if ( (operation.compare("cube")==0) && argc==7 ) {

        k=3;
    }

    return;
}