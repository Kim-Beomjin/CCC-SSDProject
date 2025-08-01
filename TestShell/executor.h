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
		"����: CCC(Clean Code Collective) \n"
		"���� : ����� / ���� : ����, ������, ������, ������\n\n"
		"Command ����:\n"
		"write: �ʿ� �Ķ���� lba(0~99 ��), data(32bit 16���� ��) - ssd�� lba cell�� data�� �����Ѵ�.\n"
		"fullwrite: �ʿ� �Ķ���� data(32bit 16���� ��) - ssd�� ��� cell�� data�� �����Ѵ�.\n"
		"read: �ʿ� �Ķ���� lba(0~99 ��) - ssd�� lba cell�� ���� �о�´�.\n"
		"fullread: �ʿ� �Ķ���� NONE - ssd�� ��� cell�� ���� �о�´�\n"
		"help: �ʿ� �Ķ���� NONE - ssd�� �ʿ��� command �� ��� ������ ���� ������ ��û�Ѵ�\n"
		"exit: �ʿ� �Ķ���� NONE - �Ϸ��� �� ������ �ٷ� �����Ѵ�\n";
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
