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

class BufferedSSD : public SSD
{
public:
	BufferedSSD();
	bool Read(LBA lba) override; //for read  
	bool Write(LBA lba, DATA data) override;
	bool Erase(LBA lba, unsigned int size) override;
	void Flush() override;
#ifdef _DEBUG
	int GetCmdCnt() { return _GetCmdCnt(); }
#endif

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
	
	SSD* ssd;
	Buffer dataBuffer[LBA_END_ADDR - LBA_START_ADDR] = { { BUF_TYPE::NONE,0 }, };
	const std::string BUFFER_DIRECTORY = "./buffer/";
	const std::string SEARCH_BUFFER_DIRECTORY = "./buffer/*";
	std::string cmdList[5];
};