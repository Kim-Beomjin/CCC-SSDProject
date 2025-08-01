#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <direct.h>
#include "executor.h"
#include "utils.h"
#include "logger.h"

bool OuputDecoratedWriter::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == WRITE_CMD && tokens.size() == NEEDED_TOKEN_COUNT) {
		if (IsValidWriteData(tokens[DATA_IDX])) return true;
		return false;
	}

	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool OuputDecoratedWriter::IsValidWriteData(const string& data) {
	if (data.size() != WRITE_DATA_SIZE) {
		print("Write Data Invalid: 10글자가 안 됨 - DATA:" + data);
		return false;
	}

	if (!(data[0] == '0' && (data[1] == 'x' || data[1] == 'X'))) {
		print("Write Data Invalid: 0x로 시작하지 않음 - DATA:" + data);
		return false;
	}

	for (size_t i = START_DATA_DIGIT_EXCEPT_HEX_PREFIX; i < WRITE_DATA_SIZE; ++i) {
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
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool FullWriter::execute(ISsdApp* app, LBA lba, DATA data)
{
	DATA fullWriteData = lba;
	for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba)
	{
		if (!Writer::execute(app, lba, fullWriteData)) return false;
	}
	print("[Full Write] Done");
	return true;
}

bool OuputDecoratedReader::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool OuputDecoratedReader::execute(ISsdApp* app, LBA lba, DATA data)
{
	if (!Reader::execute(app, lba, data)) return false;
	std::ostringstream oss;
	oss << "[Read] LBA:" << setw(2) << lba << " : " << GetResultFromFile();
	print(oss.str());
	return true;
}

bool FullReader::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
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

bool Comparer::Compare(const DATA expectedData, const string& readResult)
{
	DATA readData = TestShellUtils::stringToUnsignedInt(readResult);
	if (expectedData == readData) return true;

	SHELL_LOG("[Compare] Fail Expected ", expectedData);
	SHELL_LOG("[Compare] Fail Real ", readData);

	return false;
}


bool SizeEraser::IsValidCommand(const vector<string>& tokens)
{
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
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
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
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