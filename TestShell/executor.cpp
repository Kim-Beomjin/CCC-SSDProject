#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include "executor.h"
#include "compositExecutor.h"
#include "utils.h"

using namespace std;

IExecutor* ExecutorFactory::createExecutor(const string command)
{
	if (command == WRITE_CMD || command == FULL_WRITE_CMD) return new Writer();
	if (command == READ_CMD || command == FULL_READ_CMD) return new Reader();
	if (command == HELP_CMD) return new Helper();
	if (command == EXIT_CMD) return new Exiter();
	if (command == FIRST_SCRIPT_SHORT_NAME || command == FIRST_SCRIPT_FULL_NAME)return new FullWriteAndReadCompare(new Writer(), new Reader());
	if (command == SECOND_SCRIPT_SHORT_NAME || command == SECOND_SCRIPT_FULL_NAME) return new PartialLBAWrite(new Writer(), new Reader());
	if (command == THIRD_SCRIPT_SHORT_NAME || command == THIRD_SCRIPT_FULL_NAME) return new WriteReadAging(new Writer(), new Reader());

	return nullptr;
}

bool Writer::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == FULL_WRITE_CMD)
	{
		for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Write(lba, data);
		return true;
	}

	app->Write(lba, data);
	if (command == SCRIPT_WRITE_CMD) return true;

	cout << "[Write] Done\n";
	return true;
}

bool Reader::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == FULL_READ_CMD)
	{
		for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Read(lba);
		return true;
	}

	app->Read(lba);
	string result = GetResultFromFile();

	if (command == SCRIPT_READ_CMD)
	{
		string expected = DataToHexString(data);
		if (result == expected) return true;
#if (FIX_ME_LATER == 1)
		cout << "[Read] Expected LBA " << lba << " : " << expected << "\n";
		cout << "[Read] Real LBA " << lba << " : " << result << "\n";
#endif
		return false;
	}

	cout << "[Read] LBA " << lba << " : " << result << "\n";
	return true;
}

string Reader::GetResultFromFile(void)
{
	string result = "0x00000000";

	ifstream file(OUTPUT_NAME);
	if (!file.is_open()) file.open("../" + OUTPUT_NAME);
	if (!file.is_open()) file.open("../../" + OUTPUT_NAME);
	if (!file.is_open()) return result;

	std::stringstream buffer;
	buffer << file.rdbuf();

	result = buffer.str();

	file.close();
	return result;
}

bool Helper::execute(const string& command, LBA lba, DATA data, ISsdApp *app)
{
	cout << this->HELP_DESCRIPTION;

	return true;
}

bool Exiter::execute(const string& command, LBA lba, DATA data, ISsdApp *app)
{
	return true;
}

