#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
class P4CK
{
private: 
	char MagicBytes[4]; // "P4CK"
	uint32_t FileTable; 
	uint32_t EntriesInBytes; // / 0x10 = Entries
	uint32_t FilenameOffset;
	uint32_t FileOffset;
	uint32_t Size;
public:
	void GetMagicBytes(std::ifstream& PAK);
	uint32_t GetFileTable(std::ifstream& PAK, uint32_t Index);
	uint32_t GetEntries(std::ifstream& PAK);
	bool IsValidP4CK() const;
	void DumpFile(std::ifstream& PAK);
	//std::string Debug;
};
