#include "BufferManager.h"
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <io.h>  
#include <iostream>
#include <sstream>
#include <vector>


bool BufferManager::CheckAndReadBuffer(int lba, DATA& readData) {
	// first Load File
	return false;
}

void BufferManager::WriteBuffer(int lba, DATA writeData)
{
	//need save Algorithm
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
				ssd->Write(lba, data);
			}
			else if (command[0] == "E") {
				LBA lba = std::stoul(command[1]);
				int length = std::stoul(command[2]);
				//ssd->Erase(lba, length);
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);
}

void BufferManager::_LoadBuffer()
{
	//TODO
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
	return (filename != "." && filename != ".." && filename[0] >= '1' && filename[0] <= '5' && filename[1] == '_');
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

