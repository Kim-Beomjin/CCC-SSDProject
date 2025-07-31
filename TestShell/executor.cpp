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
	if (command == WRITE_CMD) return new Writer();
	if (command == FULL_WRITE_CMD) return new FullWriter();
	if (command == READ_CMD) return new Reader();
	if (command == FULL_READ_CMD) return new FullReader();
	if (command == ERASE_CMD) return new Eraser();
	if (command == ERASE_RANGE_CMD) return new RangeEraser();
	if (command == HELP_CMD) return new Helper();
	if (command == EXIT_CMD) return new Exiter();
	if (command == FIRST_SCRIPT_SHORT_NAME || command == FIRST_SCRIPT_FULL_NAME)return new FullWriteAndReadCompare(new Writer(), new Reader());
	if (command == SECOND_SCRIPT_SHORT_NAME || command == SECOND_SCRIPT_FULL_NAME) return new PartialLBAWrite(new Writer(), new Reader());
	if (command == THIRD_SCRIPT_SHORT_NAME || command == THIRD_SCRIPT_FULL_NAME) return new WriteReadAging(new Writer(), new Reader());

	return nullptr;
}

bool Writer::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Write(lba, data);

	cout << "[Write] Done\n";
	return true;
}

bool FullWriter::execute(ISsdApp* app, LBA lba, DATA data)
{
	bool ret = false;
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		ret = Writer::execute(app, lba, data);
		if (ret == false) return ret;
	}

	return ret;
}

bool Reader::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Read(lba);
	string result = GetResultFromFile();

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

bool FullReader::execute(ISsdApp* app, LBA lba, DATA data)
{
	bool ret = false;
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		ret = Reader::execute(app, lba, data);
		if (ret == false) return ret;
	}

	return ret;
}

bool Helper::execute(ISsdApp* app, LBA lba, DATA data)
{
	cout << this->HELP_DESCRIPTION;

	return true;
}

bool Exiter::execute(ISsdApp* app, LBA lba, DATA data)
{
	return true;
}

bool Eraser::execute(ISsdApp* app, LBA startLba, SIZE size)
{
	app->Erase(startLba, size);

	cout << "[Eraser] Done\n";
	return true;
}

bool RangeEraser::execute(ISsdApp* app, LBA startLba, LBA endLba)
{
	SIZE size = endLba - startLba + 1;

	app->Erase(startLba, size);

	cout << "[Erase_range] Done\n";
	return true;
}
