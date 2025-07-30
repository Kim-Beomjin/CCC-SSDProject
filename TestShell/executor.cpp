#include <iostream>
#include "executor.h"
#include "compositExecutor.h"

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
		cout << "[Read] Done\n";
		return true;
	}
	else if (command == "fullread")
	{
		for (DATA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Read(lba);
		return true;
	}
	return false;
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

