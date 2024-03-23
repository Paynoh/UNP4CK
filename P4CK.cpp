#include "P4CK.hpp"

void P4CK::GetMagicBytes(std::ifstream& PAK)
{
	PAK.seekg(0);
	PAK.read(this->MagicBytes, 4); 
}

uint32_t P4CK::GetFileTable(std::ifstream& PAK, uint32_t Index)
{
	PAK.seekg(4);
	char Buffer[4];
	PAK.read(Buffer, 4);
	this->FileTable = static_cast<uint32_t>(Buffer[0]) | 
		(static_cast<uint8_t>(Buffer[1]) << 8) | 
		(static_cast<uint8_t>(Buffer[2]) << 16) |
		(static_cast<uint8_t>(Buffer[3]) << 24);
	return (FileTable + Index);
}

uint32_t P4CK::GetEntries(std::ifstream& PAK)
{
    PAK.seekg(8);
    char Buffer[4];
    PAK.read(Buffer, 4);
    std::cout << std::endl;

    this->EntriesInBytes = static_cast<uint32_t>(static_cast<uint8_t>(Buffer[0])) |
        (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[1])) << 8) |
        (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[2])) << 16) |
        (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[3])) << 24);
    return this->EntriesInBytes / 0x10;
}

bool P4CK::IsValidP4CK() const
{
	if (this->MagicBytes[0] == 'P' && this->MagicBytes[1] == '4' && this->MagicBytes[2] == 'C' && this->MagicBytes[3] == 'K')
		return true;
	else
		return false;
}

void P4CK::DumpFile(std::ifstream& PAK)

{
    for (uint32_t i{ 0 }; i < this->GetEntries(PAK); i += 0x10)
    {
        char Buffer[4];
        uint32_t CurrentFileTable = this->GetFileTable(PAK, i);
        PAK.seekg(CurrentFileTable);
        PAK.read(Buffer, 4);
        this->FilenameOffset = static_cast<uint32_t>(static_cast<uint8_t>(Buffer[0])) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[1])) << 8) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[2])) << 16) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[3])) << 24);


        std::string Filename;
        char ch;
        PAK.seekg(this->FilenameOffset + CurrentFileTable);
        while (PAK.get(ch) && ch != '\0')
        { 
            Filename += ch;
        }

        std::cout << "Dumping: " << Filename << std::endl;


        PAK.seekg(CurrentFileTable + 0x4);
        PAK.read(Buffer, 4);
        this->FileOffset = static_cast<uint32_t>(static_cast<uint8_t>(Buffer[0])) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[1])) << 8) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[2])) << 16) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[3])) << 24);

        PAK.seekg(CurrentFileTable + 0x8);
        PAK.read(Buffer, 4);
        this->Size = static_cast<uint32_t>(static_cast<uint8_t>(Buffer[0])) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[1])) << 8) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[2])) << 16) |
            (static_cast<uint32_t>(static_cast<uint8_t>(Buffer[3])) << 24);

        std::vector<uint8_t> Data(this->Size);
        PAK.seekg(this->FileOffset);
        PAK.read(reinterpret_cast<char*>(Data.data()), this->Size);
        bool isFolder = (Filename.find('/') != std::string::npos);
        if (isFolder)
        {
            // Extract filename from the path
            size_t pos = Filename.find_last_of('/');
            if (pos != std::string::npos)
            {
                Filename = Filename.substr(pos + 1);
            }
        }

        std::ofstream output(Filename, std::ios::binary | std::ios::trunc);
        if (output.is_open())
        {
            output.write(reinterpret_cast<char*>(Data.data()), this->Size);
            output.close();
        }
        else
        {
            std::cerr << "Error: failed to open output file: " << Filename << std::endl;
        }
    }
}
