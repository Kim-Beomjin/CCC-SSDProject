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
	_ResetBuffer();
	if (_access(BUFFER_DIRECTORY.c_str(), 0) != 0)
	{
		int ret = _mkdir(BUFFER_DIRECTORY.c_str());
		DEBUG_ASSERT(ret, "CANNOT MAKE BUFFER DIRECTORY");
		_DumpCommand();
	}
}

bool BufferedSSD::Read(LBA lba) {
	if (SSD::IsLBAValidOrUpdateErrorToOutputFile(lba) == false)
	{
		return false;
	}
	_LoadBuffer(false /*need_delete*/);
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
	_LoadBuffer(true /*need_delete*/);
	_ConvertWriteCmdToBuf(lba, data);
	_ConvertBufToCmd();
	_DumpCommand();
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
	_LoadBuffer(true /*need_delete*/);
	_ConvertEraseCmdToBuf(lba, size);
	_ConvertBufToCmd();
	_DumpCommand();
	return true;
}

void BufferedSSD::Flush()
{
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(SEARCH_BUFFER_DIRECTORY.c_str(), &findFileData);
	int valid_file_count = 0;
	std::string filename;
	std::string parsedFilename;
	std::vector<std::string> command;
	do {
		filename = findFileData.cFileName;
		if (_CheckFileValid(filename))
		{
			parsedFilename = filename.substr(2);
			command = GlobalUtil::Split(parsedFilename, '_');
			command[command.size() - 1] = GlobalUtil::Split(command[command.size() - 1], '.')[0];
			size_t idx;
			if (command[0] == "W") {
				LBA lba = std::stoul(command[1], &idx, 10);
				DATA data = std::stoul(command[2], &idx, 16);
				SSD::Write(lba, data);
			}
			else if (command[0] == "E") {
				LBA lba = std::stoul(command[1]);
				int size = std::stoul(command[2]);
				SSD::Erase(lba, size);
			}
			if (std::remove((BUFFER_DIRECTORY + filename).c_str()) != 0) {
				DEBUG_ASSERT(false, "delete file fail");
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);

	_ResetBuffer();
	_DumpCommand();
}


void BufferedSSD::_LoadBuffer(bool need_delete)
{
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(SEARCH_BUFFER_DIRECTORY.c_str(), &findFileData);
	int valid_file_count = 0;
	std::string filename;
	std::string fullpath;
	std::vector<std::string> command;
	do {
		filename = findFileData.cFileName;
		if (_CheckFileValid(filename))
		{
			fullpath = BUFFER_DIRECTORY + filename;
			filename = filename.substr(2);
			command = GlobalUtil::Split(filename, '_');
			command[command.size() - 1] = GlobalUtil::Split(command[command.size() - 1], '.')[0];
			if (command.size() == 3) {
				_ConvertCmdToBuf(command[0], command[1], command[2]);
			}
			if (need_delete && std::remove(fullpath.c_str()) != 0) {
				DEBUG_ASSERT(false, "delete file fail");
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
}

bool BufferedSSD::_NeedFlush()
{	
	return (_GetCmdCnt() == 5);
}

int BufferedSSD::_GetCmdCnt()
{
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(SEARCH_BUFFER_DIRECTORY.c_str(), &findFileData);
	int valid_file_count = 0;
	std::string filename;
	do {
		filename = findFileData.cFileName;
		if (_CheckFileValid(filename))
		{
			if (filename.substr(2) == "empty.txt")
			{
				continue;
			}
			valid_file_count++;
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
	return valid_file_count;
}

void BufferedSSD::_ResetBuffer()
{
	for (int i = 1; i <= 5; ++i) {
		std::string filename = BUFFER_DIRECTORY + std::to_string(i) + "_empty.txt";
		cmdList[i - 1] = filename;
	}
}

void BufferedSSD::_DumpCommand()
{
	for (int i = 0; i < 5; ++i) {
		std::ofstream outFile(cmdList[i]);
		if (outFile.is_open()) {
			outFile.close();
		}
	}
	for (LBA i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		dataBuffer[i] = { BUF_TYPE::NONE, 0 };
	}
}


void BufferedSSD::_ConvertCmdToBuf(std::string cmdStr, std::string lbaStr, std::string dataStr)
{
	size_t idx;
	if (cmdStr == "W") {
		LBA lba = std::stoul(lbaStr, &idx, 10);
		DATA data = std::stoul(dataStr, &idx, 16);
		_ConvertWriteCmdToBuf(lba, data);
	}
	else if (cmdStr == "E") {
		LBA lba = std::stoul(lbaStr);
		unsigned int length = std::stoul(dataStr);
		_ConvertEraseCmdToBuf(lba, length);
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
	int erase_start_idx = -1;
	int cmd_buf_idx = 0;
	std::string filename;
	int erase_size;
	for (int i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		if (dataBuffer[i].type == BUF_TYPE::NONE)
		{
			if (erase_start_idx != -1)
			{
				erase_size = i - erase_start_idx;
				_DumpEraseCmd(cmd_buf_idx++, erase_start_idx, erase_size);
				erase_start_idx = -1;
			}
		}
		else
		{
			if (dataBuffer[i].type == BUF_TYPE::ERASE && erase_start_idx == -1) {
				erase_start_idx = i;
			}
			if (erase_start_idx != -1 && ((i - erase_start_idx + 1) % 10) == 0)
			{
				erase_size = 10;
				_DumpEraseCmd(cmd_buf_idx++, erase_start_idx, erase_size);
				erase_start_idx = -1;
			}
		}
	}
	if (erase_start_idx != -1)
	{
		erase_size = ((100 - erase_start_idx) > 10) ? 10 : 100 - erase_start_idx;
		_DumpEraseCmd(cmd_buf_idx++, erase_start_idx, erase_size);
		erase_start_idx += erase_size;
	}

	for (int i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		if (dataBuffer[i].type == BUF_TYPE::WRITE)
		{
			_DumpWriteCmd(cmd_buf_idx++, i, dataBuffer[i].data);
		}
	}
	DEBUG_ASSERT(cmd_buf_idx <= 5, "ALGORITHM ERROR!!");
}

void BufferedSSD::_DumpEraseCmd(int cmdIdx, LBA lba, int eraseSize)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_" + "E" + "_" +
		std::to_string(lba) + "_" + std::to_string(eraseSize) + ".txt";
	cmdList[cmdIdx] = filename;
}

void BufferedSSD::_DumpWriteCmd(int cmdIdx, LBA lba, DATA data)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_" + "W" + "_" +
		std::to_string(lba) + "_" + GlobalUtil::DataToHexString(data) + ".txt";
	cmdList[cmdIdx] = filename;
}


bool BufferedSSD::_CheckFileValid(const std::string& filename)
{
	return (filename[0] >= '1' && filename[0] <= '5' && filename[1] == '_');
}
