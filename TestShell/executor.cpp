#include "executor.h"
#include "compositeExecutor.h"
#include "utils.h"
#include "logger.h"

bool Writer::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Write(lba, data);
	SHELL_LOG("[Write] Done", lba, data);
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

	if (!file.is_open()) return result;

	std::stringstream buffer;
	buffer << file.rdbuf();

	result = buffer.str();

	file.close();
	return result;
}

bool Helper::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool Helper::execute(ISsdApp* app, LBA lba, DATA data)
{
	print(this->HELP_DESCRIPTION);

	return true;
}

bool Exiter::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool Exiter::execute(ISsdApp* app, LBA lba, DATA data)
{
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


bool Flusher::IsValidCommand(const vector<string>& tokens) {
	if (tokens[CMD_IDX] == CMD && tokens.size() == NEEDED_TOKEN_COUNT) return true;
	dbgPrint("Command parameter is not enough! needed param count:" + to_string(NEEDED_TOKEN_COUNT) + ", current param count:" + to_string(tokens.size()));
	return false;
}

bool Flusher::execute(ISsdApp* app, LBA lba, DATA data)
{
	app->Flush();

	print("[Flush] Done");
	return true;
}
