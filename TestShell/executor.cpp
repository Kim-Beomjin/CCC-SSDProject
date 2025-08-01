#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include "executor.h"
#include "compositExecutor.h"
#include "utils.h"
#include "logger.h"
#include <tuple>
#include <algorithm>

using namespace std;

IExecutor* ExecutorFactory::createExecutor(const string command)
{
	if (command == WRITE_CMD) return new OuputDecoratedWriter();
	if (command == FULL_WRITE_CMD) return new FullWriter();
	if (command == READ_CMD) return new OuputDecoratedReader();
	if (command == FULL_READ_CMD) return new FullReader();
	if (command == ERASE_CMD) return new Eraser();
	if (command == ERASE_RANGE_CMD) return new RangeEraser();
	if (command == HELP_CMD) return new Helper();
	if (command == EXIT_CMD) return new Exiter();
	if (command == FLUSH_CMD) return new Flusher();
	if (command == FIRST_SCRIPT_SHORT_NAME || command == FIRST_SCRIPT_FULL_NAME)return new FullWriteAndReadCompare(new Writer(), new Comparer());
	if (command == SECOND_SCRIPT_SHORT_NAME || command == SECOND_SCRIPT_FULL_NAME) return new PartialLBAWrite(new Writer(), new Comparer());
	if (command == THIRD_SCRIPT_SHORT_NAME || command == THIRD_SCRIPT_FULL_NAME) return new WriteReadAging(new Writer(), new Comparer());
	if (command == FOURTH_SCRIPT_SHORT_NAME || command == FOURTH_SCRIPT_FULL_NAME) return new EraseAndWriteAging(new Writer(), new Comparer(), new Eraser());

	return nullptr;
}

bool Writer::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Write(lba, data);
	SHELL_LOG("[Write] Done", lba, data);
	return true;
}

bool OuputDecoratedWriter::execute(ISsdApp* app, LBA lba, DATA data)
{
	if (!Writer::execute(app, lba, data)) return false;
	cout << "[Write] Done\n";
	return true;
}

bool FullWriter::execute(ISsdApp* app, LBA lba, DATA data)
{
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		if (!Writer::execute(app, lba, data)) return false;
	}
	cout << "[Full Write] Done\n";
	return true;
}

bool Reader::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Read(lba);
	SHELL_LOG("[Read] LBA ", lba, GetResultFromFile());
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

bool OuputDecoratedReader::execute(ISsdApp* app, LBA lba, DATA data)
{
	if (!Reader::execute(app, lba, data)) return false;
	cout << "[Read] LBA " << lba << " : " << GetResultFromFile() << "\n";
	return true;
}

bool FullReader::execute(ISsdApp* app, LBA lba, DATA data)
{
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		if (!OuputDecoratedReader::execute(app, lba, data)) return false;
	}
	cout << "[Full Read] Done\n";
	return true;
}

bool Comparer::execute(ISsdApp* app, LBA lba, DATA data)
{
	if (!Reader::execute(app, lba, data)) return false;
	if (!Compare(data, Reader::GetResultFromFile())) return false;
	return true;
}

bool Comparer::Compare(const DATA expectedData, const string &readResult)
{
	DATA readData = stringToUnsignedInt(readResult);
	if (expectedData == readData) return true;

	SHELL_LOG("[Compare] Fail Expected ", expectedData);
	SHELL_LOG("[Caompre] Fail Real ", readData);

	return false;
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
	std::pair<LBA, SIZE> startLbaAndCalculatedSize = calculateStartLbaAndSize(startLba, size);
	if (startLbaAndCalculatedSize.second == 0) {
		cout << "[Eraser] failed because of 0 size\n";
		return false;
	}

	if (sendEraseMessageWithCalculatedSize(app, startLbaAndCalculatedSize.first, startLbaAndCalculatedSize.second) == false) {
		cout << "[Eraser] operation failed\n";
		return false;
	}

	cout << "[Eraser] Done\n";
	return true;
}

std::pair<LBA, SIZE> Eraser::calculateStartLbaAndSize(LBA lba, SIZE size) {
	int realSize = static_cast<int>(size);
	if (realSize >= 0) return { lba, size };

	LBA savedStartLba = lba;
	SIZE calculatedSize = size;
	int calcLba = lba - abs(realSize) + 1;
	if (calcLba < 0) calcLba = 0;
	lba = calcLba;
	calculatedSize = savedStartLba - lba + 1;

	return { lba, calculatedSize };
}

bool Eraser::sendEraseMessageWithCalculatedSize(ISsdApp* app, LBA startLba, SIZE size) {
	for (int remainedSize = size; remainedSize >= 0; remainedSize -= MAX_SEND_ERASE_SIZE_FOR_ONE_TIME) {
		if (remainedSize <= MAX_SEND_ERASE_SIZE_FOR_ONE_TIME) {
			SIZE lastSize = remainedSize;
			if (app->Erase(startLba, lastSize) == false) return false;
			break;
		}

		if (app->Erase(startLba, MAX_SEND_ERASE_SIZE_FOR_ONE_TIME) == false) return false;
		startLba += MAX_SEND_ERASE_SIZE_FOR_ONE_TIME;
		if (startLba + MAX_SEND_ERASE_SIZE_FOR_ONE_TIME >= SSD_MAX_SIZE) {
			SIZE lastRemainedSize = SSD_MAX_SIZE - startLba - 1;
			if (app->Erase(startLba, lastRemainedSize) == false) return false;
			break;
		}
	}

	return true;
}

bool RangeEraser::execute(ISsdApp* app, LBA startLba, LBA endLba)
{
	LBA changedStartLba, changedEndLba;
	std::tie(changedStartLba, changedEndLba) = std::minmax(startLba, endLba);

	SIZE size = changedEndLba - changedStartLba + 1;
	cout << "call Erase::execute - changedStartLba:" << changedStartLba << ", size:" << size << ", endLba:" << changedEndLba << "startLba:" << startLba << ", endLba:" << endLba << endl;
	Eraser::execute(app, changedStartLba, size);
	return true;
}

bool Flusher::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Flush();

	cout << "[Flusher] Done\n";
	return true;
}

