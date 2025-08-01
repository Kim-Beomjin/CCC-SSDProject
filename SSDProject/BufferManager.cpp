#include "BufferManager.h"
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <io.h>  
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

BufferManager::BufferManager(SSD* ssd) : ssd{ ssd } {
	_ResetBuffer();
	if (_access(BUFFER_DIRECTORY.c_str(), 0) != 0)
	{
		int ret = _mkdir(BUFFER_DIRECTORY.c_str());
		DEBUG_ASSERT(ret, "CANNOT MAKE BUFFER DIRECTORY");
		_DumpCommand();
	}
}

bool BufferManager::BufferRead(int lba, DATA& readData) {
	_LoadBuffer(false /*need_delete*/);
	if (dataBuffer[lba].type == BUF_TYPE::NONE)
	{
		return false;
	}
	readData = dataBuffer[lba].data;
	return true;
}

void BufferManager::BufferWrite(LBA lba, DATA data)  //first i implement dumb (just want to simple I/O test)
{
	if (_NeedFlush())
	{
		BufferFlush();
	}
	_LoadBuffer(true /*need_delete*/);
	_ConvertWriteCmdToBuf(lba, data);
	_ConvertBufToCmd();
	_DumpCommand();
	
}

void BufferManager::BufferErase(LBA lba, unsigned int size)  //first i implement dumb (just want to simple I/O test)
{
	if (_NeedFlush())
	{
		BufferFlush();
	}
	_LoadBuffer(true /*need_delete*/);
	_ConvertEraseCmdToBuf(lba, size);
	_ConvertBufToCmd();
	_DumpCommand();

}

void BufferManager::BufferFlush()
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
				ssd->Write(lba, data);
			}
			else if (command[0] == "E") {
				LBA lba = std::stoul(command[1]);
				int size = std::stoul(command[2]);
				ssd->Erase(lba, size);
			}
			if (std::remove((BUFFER_DIRECTORY + filename).c_str()) != 0) {
				DEBUG_ASSERT(false, "delete file fail");
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);

	_ResetBuffer();
	_DumpCommand();
}

#ifdef _DEBUG
int BufferManager::GetCmdCnt(void)
{
	return _GetCmdCnt();
}
#endif

void BufferManager::_LoadBuffer(bool need_delete)
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

bool BufferManager::_NeedFlush()
{	
	return (_GetCmdCnt() == 5);
}

int BufferManager::_GetCmdCnt()
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

void BufferManager::_ResetBuffer()
{
	for (int i = 1; i <= 5; ++i) {
		std::string filename = BUFFER_DIRECTORY + std::to_string(i) + "_empty.txt";
		cmdList[i - 1] = filename;
	}
}

void BufferManager::_DumpCommand()
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


void BufferManager::_ConvertCmdToBuf(std::string cmdStr, std::string lbaStr, std::string dataStr)
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

void BufferManager::_ConvertWriteCmdToBuf(LBA lba, DATA data)
{
	dataBuffer[lba] = { BUF_TYPE::WRITE, data };
}

void BufferManager::_ConvertEraseCmdToBuf(LBA lba, unsigned int size)
{
	for (LBA i = lba; i < lba + size; i++) {
		dataBuffer[i] = { BUF_TYPE::ERASE, 0 };
	}
}

void BufferManager::_ConvertBufToCmd()
{
	//erase check
	int write_start_idx = -1;
	int erase_start_idx = -1;
	int erase_end_idx = -1;
	int cmd_buf_idx = 0;
	std::string filename;
	int erase_size;
	for (int i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		if (dataBuffer[i].type == BUF_TYPE::NONE)
		{
			if (erase_start_idx != -1 && erase_end_idx != -1)
			{
				erase_end_idx += 1;
				while (erase_start_idx < erase_end_idx) {
					erase_size = ((erase_end_idx - erase_start_idx) > 10) ? 10 : erase_end_idx - erase_start_idx;
					_DumpEraseCmd(cmd_buf_idx++, erase_start_idx, erase_size);
					erase_start_idx += erase_size;
				}
				erase_start_idx = -1; erase_end_idx = -1;
			}
		}
		else if (dataBuffer[i].type == BUF_TYPE::ERASE)
		{
			if (erase_start_idx == -1) {
				erase_start_idx = i;
			}
			erase_end_idx = i;
		}
		else if (dataBuffer[i].type == BUF_TYPE::WRITE)
		{
			if (((i - erase_start_idx) % 10) == 0)
			{
				erase_end_idx += 1;
				while (erase_start_idx + 10 < erase_end_idx) {
					erase_size = ((erase_end_idx - erase_start_idx) > 10) ? 10 : erase_end_idx - erase_start_idx;
					_DumpEraseCmd(cmd_buf_idx++, erase_start_idx, erase_size);
					erase_start_idx += erase_size;
				}
				erase_start_idx = -1; erase_end_idx = -1;
			}
		}
	}

	if (erase_start_idx != -1 && erase_end_idx != -1)
	{
		erase_end_idx += 1;
		while (erase_start_idx < erase_end_idx) {
			erase_size = ((erase_end_idx - erase_start_idx) > 10) ? 10 : erase_end_idx - erase_start_idx;
			_DumpEraseCmd(cmd_buf_idx++, erase_start_idx, erase_size);
			erase_start_idx += erase_size;
		}
		erase_start_idx = -1; erase_end_idx = -1;
	}

	for (int i = LBA_START_ADDR; i < LBA_END_ADDR; i++) {
		if (dataBuffer[i].type == BUF_TYPE::WRITE)
		{
			_DumpWriteCmd(cmd_buf_idx++, i, dataBuffer[i].data);
		}
	}
	DEBUG_ASSERT(cmd_buf_idx <= 5, "ALGORITHM ERROR!!");
}

void BufferManager::_DumpEraseCmd(int cmdIdx, LBA lba, int eraseSize)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_" + "E" + "_" +
		std::to_string(lba) + "_" + std::to_string(eraseSize) + ".txt";
	cmdList[cmdIdx] = filename;
}

void BufferManager::_DumpWriteCmd(int cmdIdx, LBA lba, DATA data)
{
	std::string filename = BUFFER_DIRECTORY + std::to_string(cmdIdx + 1) + "_" + "W" + "_" +
		std::to_string(lba) + "_" + GlobalUtil::DataToHexString(data) + ".txt";
	cmdList[cmdIdx] = filename;
}


bool BufferManager::_CheckFileValid(const std::string& filename)
{
	return (filename[0] >= '1' && filename[0] <= '5' && filename[1] == '_');
}
