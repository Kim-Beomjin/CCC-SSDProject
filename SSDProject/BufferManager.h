#pragma once

#include <vector>
#include "ssd.h"

enum BUF_TYPE
{
	NONE,
	WRITE,
	ERASE
};
struct Buffer
{
	BUF_TYPE type;
	DATA data;
};
class BufferManager
{
public:
	BufferManager(SSD* ssd) : ssd{ ssd } {}
	bool CheckAndReadBuffer(int lba, DATA& readData); //for read  
	void WriteBuffer(int lba, DATA writeData);
	void Flush();

private:
	bool _NeedFlush();
	void _LoadBuffer();
	bool _CheckFileValid(const std::string& filename);
	bool _CheckDirectoryInvalid();
	void _CreateDirectory();
	std::vector<std::string> _Split(const std::string& str, const char delemeter);
	SSD* ssd;
	Buffer dataBuffer[LBA_END_ADDR - LBA_START_ADDR];
	const std::string BUFFER_DIRECTORY = "./buffer/";
	const std::string SEARCH_BUFFER_DIRECTORY = "./buffer/*";
};