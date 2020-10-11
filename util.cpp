#include "util.h"

uint32_t swap_endian(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

void read_file(std::string file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in | std::ios::binary);

    if (file.is_open())
    {
        uint32_t magic_num = 0, num_of_images = 0, rows = 0, columns = 0;

        file.read(reinterpret_cast<char*>(&magic_num), 4);
        magic_num = swap_endian(magic_num);
        std::cout << "magic num: " << magic_num << std::endl;

        file.read(reinterpret_cast<char*>(&num_of_images), 4);
        num_of_images = swap_endian(num_of_images);
        std::cout << "#images: " << num_of_images << std::endl;

        file.read(reinterpret_cast<char*>(&rows), 4);
        rows = swap_endian(rows);
        std::cout << "#rows: " << rows << std::endl;

        file.read(reinterpret_cast<char*>(&columns), 4);
        columns = swap_endian(columns);
        std::cout << "#columns: " << columns << std::endl;

        for (uint32_t i = 0; i < num_of_images; ++i)
        {
            for (uint32_t r = 0; r < rows; ++r)
            {
                for (uint32_t c = 0; c < columns; ++c)
                {
                    unsigned char temp = 0;
                    file.read((char *)&temp, sizeof(temp));
                    std::cout << (double) temp << std::endl;
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