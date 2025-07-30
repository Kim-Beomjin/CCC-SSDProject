#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include "executor.h"

using namespace std;

IExecutor* ExecutorFactory::createExecutor(const string command)
{
	if (command == "write" || command == "fullwrite") return new Writer();
	if (command == "read" || command == "fullread") return new Reader();
	if (command == "help") return new Helper();
	if (command == "exit") return new Exiter();

	return nullptr;
}

bool Writer::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == "write")
	{
		app->Write(lba, data);
		cout << "[Write] Done\n";

		return true;
	}
	else if (command == "fullwrite")
	{
		for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Write(lba, data);
		return true;
	}
	return false;
}

bool Reader::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == "read")
	{
		app->Read(lba);

		string result = GetResultFromFile();
		cout << "[Read] LBA " << lba << " : " << result << "\n";
		return true;
	}
	else if (command == "fullread")
	{
		for (DATA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Read(lba);
		return true;
	}
	return false;
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

