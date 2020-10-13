#include "util.h"

uint32_t swap_endian(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

void read_file(std::string file_name)
{
    std::ifstream file;                                                             //Stream class to read from files
    file.open(file_name, std::ios::in | std::ios::binary);                          // in(open for input operations), binary(open in binary mode)

    if (file.is_open())                                                             //If the file has successfully opened
    {
        uint32_t magic_num = 0, num_of_images = 0, rows = 0, columns = 0;           //uint32_t => unsigned bit, [ 0, 2^32 -1 ]. That's the type of the magic num, noi,r, c, according to the assignment.

        read_metadata(file, magic_num, num_of_images, rows, columns, 4);

        for (uint32_t i = 0; i < num_of_images; ++i)
        {
            for (uint32_t r = 0; r < rows; ++r)
            {
                for (uint32_t c = 0; c < columns; ++c)
                {
                    unsigned char temp = 0;
                    file.read((char *)&temp, sizeof(temp));
                    // std::cout << (double) temp << std::endl;                     //We have only int input, we are not going to need (double) cast.
                    std::cout << (int) temp << std::endl;
                }
            }
        }
    }
    else
    {
        std::cout << "Something went wrong while reading the file " << file_name << std::endl;
        exit(EXIT_FAILURE);
    }
}

void read_metadata(std::ifstream& file, uint32_t& magic_num, uint32_t& num_of_images, uint32_t& rows, uint32_t& columns,int size){              //Take the metadata (magic number, number of images, rows and columns)

    file.read(reinterpret_cast<char*>(&magic_num), size);                                                                                       //we are doing this type of cast bcz we are playing with bits, and bcz we want to convet: 0x00000803 -> 2051
    magic_num = swap_endian(magic_num);
    std::cout << "magic num: " << magic_num << std::endl;

    file.read(reinterpret_cast<char*>(&num_of_images), size);
    num_of_images = swap_endian(num_of_images);
    std::cout << "#images: " << num_of_images << std::endl;

    file.read(reinterpret_cast<char*>(&rows), size);
    rows = swap_endian(rows);
    std::cout << "#rows: " << rows << std::endl;

    file.read(reinterpret_cast<char*>(&columns), size);
    columns = swap_endian(columns);
    std::cout << "#columns: " << columns << std::endl;

    std::cout<<"mounaqi"<< std::endl;

}  
