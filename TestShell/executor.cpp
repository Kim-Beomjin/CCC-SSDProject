#include <fstream>
#include <sstream>
#include <direct.h>
#include <tuple>
#include <algorithm>
#include "executor.h"
#include "compositExecutor.h"
#include "utils.h"
#include "logger.h"

using namespace std;

IExecutor* ExecutorFactory::createExecutor(const string command)
{
	if (command == WRITE_CMD) return new OuputDecoratedWriter();
	if (command == FULL_WRITE_CMD) return new FullWriter();
	if (command == READ_CMD) return new OuputDecoratedReader();
	if (command == FULL_READ_CMD) return new FullReader();
	if (command == ERASE_CMD) return new SizeEraser();
	if (command == ERASE_RANGE_CMD) return new RangeEraser();
	if (command == HELP_CMD) return new Helper();
	if (command == EXIT_CMD) return new Exiter();
	if (command == FLUSH_CMD) return new Flusher();
	if (command == FIRST_SCRIPT_SHORT_NAME || command == FIRST_SCRIPT_FULL_NAME) return new FullWriteAndReadCompare(new Writer(), new Comparer());
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

bool OuputDecoratedWriter::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == WRITE_CMD && tokens.size() == NEEDED_TOKEN_COUNT) {
		if (IsValidWriteData(tokens[DATA_IDX])) return true;
		return false;
	}

	print("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT ) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool OuputDecoratedWriter::IsValidWriteData(const string& data) {
	if (data.size() != 10) {
		print("Write Data Invalid: 10글자가 안 됨 - DATA:" + data);
		return false;
	}

	if (!(data[0] == '0' && (data[1] == 'x' || data[1] == 'X'))) {
		print("Write Data Invalid: 0x로 시작하지 않음 - DATA:" + data);
		return false;
	}

	for (size_t i = 2; i < 10; ++i) {
		if (!std::isxdigit(static_cast<unsigned char>(data[i]))) {
			print("Write Data Invalid: 숫자가 아닌 값이 들어가 있음 - DATA:" + data);
			return false;
		}
	}

	return true;
}

bool OuputDecoratedWriter::execute(ISsdApp* app, LBA lba, DATA data)
{
	if (!Writer::execute(app, lba, data)) return false;
	print("[Write] Done");
	return true;
}

bool FullWriter::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	print("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT ) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool FullWriter::execute(ISsdApp* app, LBA lba, DATA data)
{
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		if (!Writer::execute(app, lba, data)) return false;
	}
	print("[Full Write] Done");
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

bool OuputDecoratedReader::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	print("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT ) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool OuputDecoratedReader::execute(ISsdApp* app, LBA lba, DATA data)
{
	if (!Reader::execute(app, lba, data)) return false;
	std::ostringstream oss;
	oss << "[Read] LBA:" << setw(2) << lba << ":" << GetResultFromFile();
	print(oss.str());
	return true;
}

bool FullReader::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	print("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT ) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool FullReader::execute(ISsdApp* app, LBA lba, DATA data)
{
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		if (!OuputDecoratedReader::execute(app, lba, data)) return false;
	}
	print("[Full Read] Done");
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

bool Helper::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	print("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT ) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool Helper::execute(ISsdApp* app, LBA lba, DATA data)
{
	print(this->HELP_DESCRIPTION);

	return true;
}

bool Exiter::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	print("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT ) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool Exiter::execute(ISsdApp* app, LBA lba, DATA data)
{
	print("[Exit] Done");
	return true;
}

bool Eraser::execute(ISsdApp* app, LBA lba, SIZE size)
{
	if (!ExecuteChunkedErase(app, lba, size)) return false;
	SHELL_LOG("[Erase] Done", lba, size);
	return true;
}

bool Eraser::ExecuteChunkedErase(ISsdApp* app, LBA startLba, SIZE size)
{
	SIZE maxEndLba = min(startLba + size, SSD_MAX_SIZE);

	LBA currentLba = startLba;
	while (currentLba < maxEndLba) {
		SIZE eraseSize = min(maxEndLba - currentLba, SSD_ERASE_MAX_SIZE);

		if (!app->Erase(currentLba, eraseSize)) return false;
		SHELL_LOG("[Chuck Erase] Done", currentLba, eraseSize);

		currentLba += eraseSize;
	}
	return true;
}

bool SizeEraser::IsValidCommand(const vector<string>& tokens)
{
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	return false;
}

bool SizeEraser::execute(ISsdApp* app, LBA startLba, SIZE size)
{
	if (size == 0) return true;

	pair<LBA, SIZE> eraseRange = CalculateStartLbaAndSize(startLba, size);

	if (!Eraser::execute(app, eraseRange.first, eraseRange.second))
	{
		print("[Erase] Fail");
		return false;
	}

	print("[Erase] Done");
	return true;
}

pair<LBA, SIZE> SizeEraser::CalculateStartLbaAndSize(LBA lba, SIZE size)
{
	int realSize = static_cast<int>(size);
	if (realSize >= 0) return { lba, size };

	int savedStartLba = static_cast<int>(lba);
	LBA newStartLba = static_cast<LBA>(max(savedStartLba + realSize + 1, 0));
	SIZE newSize = static_cast<SIZE>(savedStartLba - newStartLba + 1);

	return { newStartLba, newSize };
}

bool RangeEraser::IsValidCommand(const vector<string>& tokens)
{
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	return false;
}

bool RangeEraser::execute(ISsdApp* app, LBA startLba, LBA endLba)
{
	int realEndLba = static_cast<int>(endLba);
	if (realEndLba < 0) return false;

	pair<LBA, SIZE> eraseRange = GetOrderedLbaRange(startLba, endLba);

	if (!Eraser::execute(app, eraseRange.first, eraseRange.second))
	{
		print("[Erase Range] Fail");
		return false;
	}

	print("[Erase Range] Done");
	return true;
}

pair<LBA, SIZE> RangeEraser::GetOrderedLbaRange(LBA startLba, LBA endLba)
{
	LBA changedStartLba, changedEndLba;
	std::tie(changedStartLba, changedEndLba) = std::minmax(startLba, endLba);

	SIZE size = changedEndLba - changedStartLba + 1;

	return { changedStartLba, size };
}

bool Flusher::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	return false;
}

bool Flusher::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Flush();

	print("[Flush] Done");
	return true;
}
