#pragma once
#include "global_config.h"
#include "interface.h"

class Writer : public IExecutor
{
public:
	bool execute(ISsdApp* app, const string& command, LBA lba, DATA data) override;
};

class Reader : public IExecutor
{
public:
	bool execute(ISsdApp* app, const string& command, LBA lba, DATA data) override;
	string GetResultFromFile(void);

	const string OUTPUT_NAME = "ssd_output.txt";

};

class Helper : public IExecutor
{
public:
	bool execute(ISsdApp* app, const string& command, LBA lba, DATA data) override;
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
};

class Exiter : public IExecutor {
public:
	bool execute(ISsdApp* app, const string& command, LBA lba, DATA data) override;
};