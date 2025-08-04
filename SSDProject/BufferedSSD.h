#pragma once

#include <vector>
#include <utility>
#include "ssd.h"

class BufferedSSD : public SSD
{
public:
	BufferedSSD();
	~BufferedSSD() override;
	bool Read(LBA lba) override; //for read  
	bool Write(LBA lba, DATA data) override;
	bool Erase(LBA lba, unsigned int size) override;
	void Flush() override;

private:
	bool _NeedFlush();
	void _LoadCommand();
	
	bool _CheckFileValid(const std::string& filename);
	
	void _LoadParsedCommand(std::string filename);

	void _DumpCommand();
	void _ConvertBufToCmd();

	void _ConvertCmdToBuf();
	void _ConvertWriteCmdToBuf(LBA lba, DATA data);
	void _ConvertEraseCmdToBuf(LBA lba, unsigned int size);

	void _DumpEraseCmd(int cmdIdx, LBA lba, int eraseSize);
	void _DumpWriteCmd(int cmdIdx, LBA lba, DATA data);
	void _DumpEmpty(int cmdIdx);

	int _GetCmdCnt();
	
	SSD* ssd;
	Buffer dataBuffer[LBA_END_ADDR - LBA_START_ADDR] = { { BUF_TYPE::NONE,0 }, };
	const std::string BUFFER_DIRECTORY = "./buffer/";
	const std::string SEARCH_BUFFER_DIRECTORY = "./buffer/*";
	const int BUFFER_IDX = 0;
	const int COMMAND_OPCODE = 1;
	const int DWORD1 = 2;
	const int DWORD2 = 3;
	const int MAX_BUFFER_SIZE = 5;
	const unsigned int INVALID_IDX = 0xFFFFFFFF;
	std::vector<std::pair<std::string, Command>> cmdList;
};