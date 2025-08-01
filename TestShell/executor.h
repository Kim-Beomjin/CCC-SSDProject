#pragma once
#include "global_config.h"
#include "interface.h"

class Writer : public IExecutor
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class OuputDecoratedWriter : public Writer
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
private:
	bool IsValidWriteData(const string& data);
	const string CMD = WRITE_CMD;
	const int NEEDED_TOKEN_COUNT = 3;
};

class FullWriter : public Writer
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
private:
	const string CMD = FULL_WRITE_CMD;
	const int NEEDED_TOKEN_COUNT = 2;
};

class Reader : public IExecutor
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data = 0) override;

	string GetResultFromFile(void);
private:
	const string OUTPUT_NAME = "ssd_output.txt";
};

class OuputDecoratedReader : public Reader
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
private:
	const string CMD = READ_CMD;
	const int NEEDED_TOKEN_COUNT = 2;
};

class FullReader : public OuputDecoratedReader
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
private:
	const string CMD = FULL_READ_CMD;
	const int NEEDED_TOKEN_COUNT = 1;
};

class Comparer : public Reader
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data) override;

private:
	bool Compare(const DATA expectedData, const string& readResult);
};

class Helper : public IExecutor
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba = 0, DATA data = 0) override;
private:
	const std::string HELP_DESCRIPTION =
		"팀명: CCC(Clean Code Collective) \n"
		"팀장 : 김범진 / 팀원 : 김경민, 김윤진, 김율곤, 정지윤\n\n"
		"Command 설명:\n"
		"write: 필요 파라미터 lba(0~99 중), data(32bit 16진수 값) - ssd의 lba cell에 data를 저장한다.\n"
		"fullwrite: 필요 파라미터 data(32bit 16진수 값) - ssd의 모든 cell에 data를 저장한다.\n"
		"read: 필요 파라미터 lba(0~99 중) - ssd의 lba cell의 값을 읽어온다.\n"
		"fullread: 필요 파라미터 NONE - ssd의 모든 cell의 값을 읽어온다\n"
		"help: 필요 파라미터 NONE - ssd에 필요한 command 등 모든 정보에 대한 도움말을 요청한다\n"
		"exit: 필요 파라미터 NONE - 하려던 거 끝내고 바로 종료한다\n";
	const string CMD = HELP_CMD;
	const int NEEDED_TOKEN_COUNT = 1;
};

class Exiter : public IExecutor
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba = 0, DATA data = 0) override;
private:
	const string CMD = EXIT_CMD;
	const int NEEDED_TOKEN_COUNT = 1;
};

class Eraser : public IExecutor
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba = 0, SIZE size = 0) override;
private:
	bool sendEraseMessageWithCalculatedSize(ISsdApp* app, LBA startLba, SIZE size);
	std::pair<LBA, SIZE> calculateStartLbaAndSize(LBA lba, SIZE size);
	const string CMD = ERASE_CMD;
	const int NEEDED_TOKEN_COUNT = 3;
};

class RangeEraser : public Eraser
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA startLba = 0, LBA endLba = 0) override;
private:
	const string CMD = ERASE_RANGE_CMD;
	const int NEEDED_TOKEN_COUNT = 3;
};

class Flusher : public IExecutor
{
public:
	bool IsValidCommand(const vector<string>& tokens) override;
	bool execute(ISsdApp* app, LBA lba = 0, DATA data = 0) override;
private:
	const string CMD = FLUSH_CMD;
	const int NEEDED_TOKEN_COUNT = 1;
};
