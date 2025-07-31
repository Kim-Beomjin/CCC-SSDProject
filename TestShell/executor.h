#pragma once
#include "global_config.h"
#include "interface.h"

class Writer : public IExecutor
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class FullWriter : public Writer
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class Reader : public IExecutor
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data = 0) override;

	string GetResultFromFile(void);
private:
	const string OUTPUT_NAME = "ssd_output.txt";
};

class FullReader : public Reader
{
public:
	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class Helper : public IExecutor
{
public:
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
};

class Exiter : public IExecutor {
public:
	bool execute(ISsdApp* app, LBA lba = 0, DATA data = 0) override;
};

class Eraser : public IExecutor {
public:
	bool execute(ISsdApp* app, LBA lba = 0, DATA data = 0) override;
};

class RangeEraser : public IExecutor {
public:
	bool execute(ISsdApp* app, LBA lba = 0, DATA data = 0) override;
};
