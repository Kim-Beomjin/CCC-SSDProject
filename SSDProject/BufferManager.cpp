#include "BufferManager.h"
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <io.h>  
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio> 


bool BufferManager::CheckAndReadBuffer(int lba, DATA& readData) {
	if (_CheckDirectoryInvalid()) //if we dont have a buffer directory 
	{
		_CreateDirectory();
	}
	_LoadBuffer();
	if (dataBuffer[lba].type == BUF_TYPE::NONE)
	{
		return false;
	}
	readData = dataBuffer[lba].data;
	return true;
}

void BufferManager::WriteBuffer(std::string cmd, std::string lba, std::string data)  //first i implement dumb (just want to simple I/O test)
{
	//need save Algorithm
	if (_CheckDirectoryInvalid()) //if we dont have a buffer directory 
	{
		_CreateDirectory();
	}
	if (_NeedFlush())
	{
		Flush();
	}
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(SEARCH_BUFFER_DIRECTORY.c_str(), &findFileData);
	std::string filename, parsedFilename;
	int next_file_index = 1;
	int valid_file_index = 1;
	do {
		filename = findFileData.cFileName;
		if (_CheckFileValid(filename))
		{
			next_file_index = filename[0] - '0';
			if (filename.substr(2) == "empty.txt")
			{
				std::string full_path = BUFFER_DIRECTORY + filename;
				{
					if (std::remove(full_path.c_str()) != 0) {
						DEBUG_ASSERT(false, "delete file fail");
					}
				}
				break;
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
	
	std::string newFilename = BUFFER_DIRECTORY + filename[0] + "_" + cmd + "_" + lba + "_" + data + ".txt";
	std::ofstream outFile(newFilename);
	if (outFile.is_open()) {
		outFile.close();
	}
}

void BufferManager::Flush()
{
	if (_CheckDirectoryInvalid()) //if we dont have a buffer directory 
	{
		return;
	}
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
			command = _Split(parsedFilename, '_');
			command[command.size() - 1] = _Split(command[command.size() - 1], '.')[0];
			size_t idx;
			if (command[0] == "W") {
				LBA lba = std::stoul(command[1], &idx, 10);
				DATA data = std::stoul(command[2], &idx, 16);
				ssd->Write(lba, data);
			}
			else if (command[0] == "E") {
				LBA lba = std::stoul(command[1]);
				int size = std::stoul(command[2]);
				//ssd->Erase(lba, size);
			}
			if (std::remove((BUFFER_DIRECTORY + filename).c_str()) != 0) {
				DEBUG_ASSERT(false, "delete file fail");
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);

	_CreateDirectory();
}

void BufferManager::_LoadBuffer()
{
	if (_CheckDirectoryInvalid()) //if we dont have a buffer directory 
	{
		return;
	}
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(SEARCH_BUFFER_DIRECTORY.c_str(), &findFileData);
	int valid_file_count = 0;
	std::string filename;
	std::vector<std::string> command;
	do {
		filename = findFileData.cFileName;
		if (_CheckFileValid(filename))
		{
			filename = filename.substr(2);
			command = _Split(filename, '_');
			command[command.size() - 1] = _Split(command[command.size() - 1], '.')[0];
			size_t idx;
			if (command[0] == "W") {
				LBA lba = std::stoul(command[1], &idx, 10);
				DATA data = std::stoul(command[2], &idx, 16);
				dataBuffer[lba] = { BUF_TYPE::WRITE, data };
			}
			else if (command[0] == "E") {
				LBA lba = std::stoul(command[1]);
				unsigned int length = std::stoul(command[2]);
				for (LBA i = lba; i < lba + length; i++) {
					dataBuffer[i] = { BUF_TYPE::ERASE, 0 };
				}
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
}

bool BufferManager::_NeedFlush()
{
	if (_CheckDirectoryInvalid()) //if we dont have a directory 
	{
		return false;
	}
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
	
	return (valid_file_count == 5);
}

bool BufferManager::_CheckFileValid(const std::string& filename)
{
	return (filename[0] >= '1' && filename[0] <= '5' && filename[1] == '_');
}

bool BufferManager::_CheckDirectoryInvalid()
{
	return (_access(BUFFER_DIRECTORY.c_str(), 0) != 0);
	
}

void BufferManager::_CreateDirectory()
{
	int ret = _mkdir(BUFFER_DIRECTORY.c_str());

	for (int i = 1; i <= 5; ++i) {
		std::string filename = BUFFER_DIRECTORY + "/" + std::to_string(i) + "_empty.txt";
		std::ofstream outFile(filename);
		if (outFile.is_open()) {
			outFile.close();
		}
	}
}

std::vector<std::string> BufferManager::_Split(const std::string& str, const char delimiter) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;

	while (std::getline(ss, item, delimiter)) {
		result.push_back(item);
	}
	return result;
}

