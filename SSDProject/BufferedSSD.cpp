#include "BufferedSSD.h"
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <io.h>  
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

BufferedSSD::BufferedSSD() {
	if (_access(BUFFER_DIRECTORY.c_str(), 0) != 0)
	{
		int ret = _mkdir(BUFFER_DIRECTORY.c_str());
		DEBUG_ASSERT(ret, "CANNOT MAKE BUFFER DIRECTORY");
	}
	_LoadCommand();

}

BufferedSSD::~BufferedSSD()
{
	_DumpCommand();
}

bool BufferedSSD::Read(LBA lba) {
	if (SSD::IsLBAValidOrUpdateErrorToOutputFile(lba) == false)
	{
		return false;
	}
	_ConvertCmdToBuf();
	if (dataBuffer[lba].type == BUF_TYPE::NONE)
	{
		SSD::Read(lba);
		return true;
	}
	SSD::UpdateOutputFileUsingData(dataBuffer[lba].data);
	return true;
}

bool BufferedSSD::Write(LBA lba, DATA data)  //first i implement dumb (just want to simple I/O test)
{
	if (SSD::IsLBAValidOrUpdateErrorToOutputFile(lba) == false)
	{
		return false;
	}
	if (_NeedFlush())
	{
		Flush();
	}
	_ConvertCmdToBuf();
	_ConvertWriteCmdToBuf(lba, data);
	_ConvertBufToCmd();
	return true;
}

bool BufferedSSD::Erase(LBA lba, unsigned int size)  //first i implement dumb (just want to simple I/O test)
{
	if (SSD::IsLBAValidOrUpdateErrorToOutputFile(lba, size) == false)
	{
		return false;
	}
	if (_NeedFlush())
	{
		Flush();
	}
	_ConvertCmdToBuf();
	_ConvertEraseCmdToBuf(lba, size);
	_ConvertBufToCmd();
	return true;
}

void BufferedSSD::Flush()
{
	int idx = 0;
	for (idx = 0; idx < cmdList.size(); idx++)
	{
		if (cmdList[idx].first == "W") {
			SSD::Write(cmdList[idx].second.dword1, cmdList[idx].second.dword2);
		}
		else if (cmdList[idx].first == "E") {
			SSD::Erase(cmdList[idx].second.dword1, cmdList[idx].second.dword2);
		}
	}
	cmdList.clear();
	for (int i = 0; i < 100; i++) {
		dataBuffer[i] = { BUF_TYPE::NONE, 0 };
	}
}

void BufferedSSD::_LoadCommand()
{
	cmdList.clear();
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(SEARCH_BUFFER_DIRECTORY.c_str(), &findFileData);
	std::string filename;
	std::string fullpath;
	do {
		filename = findFileData.cFileName;
		if (_CheckFileValid(filename))
		{
			fullpath = BUFFER_DIRECTORY + filename;
			_LoadParsedCommand(filename);
			if (std::remove(fullpath.c_str()) != 0) {
				DEBUG_ASSERT(false, "delete file fail");
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
}

void BufferedSSD::_LoadParsedCommand(std::string filename)
{
	std::vector<std::string> command;
	command = GlobalUtil::Split(filename, '_');
	command[command.size() - 1] = GlobalUtil::Split(command[command.size() - 1], '.')[0]; //remove .txt
	if (command.size() != 4)
	{
		return;
	}
	unsigned int idx = GlobalUtil::SafeStoul(command[BUFFER_IDX].c_str(), DECIMAL_BASE);
	if (command[COMMAND_OPCODE] == "W")
	{
		LBA lba = GlobalUtil::SafeStoul(command[DWORD1].c_str(), DECIMAL_BASE);
		DATA data = GlobalUtil::SafeStoul(command[DWORD2].c_str(), HEXA_BASE);
		cmdList.push_back({ "W", {lba, data } });
	}
	else if (command[COMMAND_OPCODE] == "E")
	{
		LBA lba = GlobalUtil::SafeStoul(command[DWORD1].c_str(), DECIMAL_BASE);
		unsigned int size = GlobalUtil::SafeStoul(command[DWORD2].c_str(), DECIMAL_BASE);
		cmdList.push_back({ "W", {lba, size }});
	}
}


void BufferedSSD::_DumpCommand()
{

	int idx = 0;
	for (idx = 0; idx < cmdList.size(); idx++)
	{
		if (cmdList[idx].first == "W") {
			_DumpWriteCmd(idx, cmdList[idx].second.dword1, cmdList[idx].second.dword2);
		}
		else if (cmdList[idx].first == "E") {
			_DumpEraseCmd(idx, cmdList[idx].second.dword1, cmdList[idx].second.dword2);
		}
	}
	for (;idx < MAX_BUFFER_SIZE; idx++)
	{
		_DumpEmpty(idx);
	}
}

void BufferedSSD::_DumpEraseCmd(int cmdIdx, LBA lba, int eraseSize)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_" + "E" + "_" +
		std::to_string(lba) + "_" + std::to_string(eraseSize) + ".txt";
	std::ofstream outFile(filename);
	if (outFile.is_open())
	{
		outFile.close();
	}
}

void BufferedSSD::_DumpWriteCmd(int cmdIdx, LBA lba, DATA data)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_" + "W" + "_" +
		std::to_string(lba) + "_" + GlobalUtil::DataToHexString(data) + ".txt";
	std::ofstream outFile(filename);
	if (outFile.is_open())
	{
		outFile.close();
	}
}

void BufferedSSD::_DumpEmpty(int cmdIdx)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_empty.txt";
	std::ofstream outFile(filename);
	if (outFile.is_open())
	{
		outFile.close();
	}
}

bool BufferedSSD::_CheckFileValid(const std::string& filename)
{
	return (filename[0] >= '1' && filename[0] <= '5' && filename[1] == '_');
}

bool BufferedSSD::_NeedFlush()
{
	return (cmdList.size() == 5);
}

int BufferedSSD::_GetCmdCnt()
{
	return static_cast<int>(cmdList.size());
}


// TODO :: Convert cmd can be starategy pattern

void BufferedSSD::_ConvertCmdToBuf()
{
	for (int i = 0; i < cmdList.size(); i++) {
		size_t idx;
		std::string cmdStr = cmdList[i].first;
		if (cmdStr == "W") {
			_ConvertWriteCmdToBuf(cmdList[i].second.dword1, cmdList[i].second.dword2);
		}
		else if (cmdStr == "E") {
			_ConvertEraseCmdToBuf(cmdList[i].second.dword1, cmdList[i].second.dword2);
		}
	}
}

void BufferedSSD::_ConvertWriteCmdToBuf(LBA lba, DATA data)
{
	dataBuffer[lba] = { BUF_TYPE::WRITE, data };
}

void BufferedSSD::_ConvertEraseCmdToBuf(LBA lba, unsigned int size)
{
	for (LBA i = lba; i < lba + size; i++) {
		dataBuffer[i] = { BUF_TYPE::ERASE, 0 };
	}
}

void BufferedSSD::_ConvertBufToCmd()
{
	//erase check
	cmdList.clear();
	LBA erase_start_idx = INVALID_IDX;
	int cmd_buf_idx = 0;
	std::string filename;
	unsigned int erase_size;
	for (int i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		if (dataBuffer[i].type == BUF_TYPE::NONE)
		{
			if (erase_start_idx != INVALID_IDX)
			{
				erase_size = i - erase_start_idx;
				cmdList.push_back({ "E", { erase_start_idx, erase_size } });
				erase_start_idx = INVALID_IDX;
			}
		}
		else
		{
			if (dataBuffer[i].type == BUF_TYPE::ERASE && erase_start_idx == -1) {
				erase_start_idx = i;
			}
			if (erase_start_idx != INVALID_IDX && ((i - erase_start_idx + 1) % 10) == 0)
			{
				erase_size = 10;
				cmdList.push_back({ "E", { erase_start_idx, erase_size } });
				erase_start_idx = INVALID_IDX;
			}
		}
	}
	if (erase_start_idx != INVALID_IDX)
	{
		erase_size = ((100 - erase_start_idx) > 10) ? 10 : 100 - erase_start_idx;
		cmdList.push_back({ "E", { erase_start_idx, erase_size } });
		erase_start_idx = INVALID_IDX;
	}

	for (LBA i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		if (dataBuffer[i].type == BUF_TYPE::WRITE)
		{
			cmdList.push_back({ "W", { i, dataBuffer[i].data } });
		}
	}
	DEBUG_ASSERT(cmd_buf_idx <= 5, "ALGORITHM ERROR!!");
}



