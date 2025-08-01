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
	BufferManager(SSD* ssd);
	bool BufferRead(int lba, DATA& readData); //for read  
	void BufferWrite(LBA lba, DATA data);
	void BufferErase(LBA lba, unsigned int size);
	void BufferFlush();

private:
	bool _NeedFlush();
	void _LoadBuffer(bool need_delete);
	
	bool _CheckFileValid(const std::string& filename);
	void _ResetBuffer();
	
	void _DumpCommand();
	void _ConvertBufToCmd();

	void _ConvertCmdToBuf(std::string cmdStr, std::string lbaStr, std::string dataStr);
	void _ConvertWriteCmdToBuf(LBA lba, DATA data);
	void _ConvertEraseCmdToBuf(LBA lba, unsigned int size);

	void _DumpEraseCmd(int cmdIdx, LBA lba, int eraseSize);
	void _DumpWriteCmd(int cmdIdx, LBA lba, DATA data);

	int _GetCmdCnt();
	
	std::vector<std::string> _Split(const std::string& str, const char delemeter);

	SSD* ssd;
	Buffer dataBuffer[LBA_END_ADDR - LBA_START_ADDR] = { { BUF_TYPE::NONE,0 }, };
	const std::string BUFFER_DIRECTORY = "./buffer/";
	const std::string SEARCH_BUFFER_DIRECTORY = "./buffer/*";
	std::string cmdList[5];
};