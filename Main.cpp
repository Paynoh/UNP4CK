#include <iostream>
#include "P4CK.hpp"
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return -1;
    }

    const char* FileName = argv[1];
    std::ifstream PAK(FileName, std::ios::binary);
    if (!PAK.is_open())
    {
        std::cout << "Error while opening " << FileName << '\n';
        return -1;
    }

    P4CK CPak{};
    CPak.GetMagicBytes(PAK);
    if (!CPak.IsValidP4CK())
    {
        std::cout << FileName << " is not a valid PAK file (bad signature)\n";
        return -1;
    }
    std::cout << "PAK is valid!!\n";
    CPak.DumpFile(PAK);
    return 0;
}