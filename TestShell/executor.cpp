#include <iostream>
#include "executor.h"

using namespace std;

IExecutor* ExecutorFactory::createExecutor(const string command)
{
	if (command == "write" || command == "fullwrite") return new Writer();
	if (command == "read" || command == "fullread") return new Reader();
	if (command == "help") return new Helper();
	if (command == "exit") return new Exiter();

	return nullptr;
}

bool Writer::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == "write")
	{
		app->Write(lba, data);
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
	cout << "팀명: CCC(Clean Code Collective) \n";
	cout << "팀장 : 김범진 / 팀원 : 김경민, 김윤진, 김율곤, 정지윤\n\n";
	cout << "Command 설명:\n";
	cout << "write: 필요 파라미터 lba(0~99 중), data(32bit 16진수 값) - ssd의 lba cell에 data를 저장한다.\n";
	cout << "fullwrite: 필요 파라미터 data(32bit 16진수 값) - ssd의 모든 cell에 data를 저장한다.\n";
	cout << "read: 필요 파라미터 lba(0~99 중) - ssd의 lba cell의 값을 읽어온다.\n";
	cout << "fullread: 필요 파라미터 NONE - ssd의 모든 cell의 값을 읽어온다\n";
	cout << "help: 필요 파라미터 NONE - ssd에 필요한 command 등 모든 정보에 대한 도움말을 요청한다\n";
	cout << "exit: 필요 파라미터 NONE - 하려던 거 끝내고 바로 종료한다\n" << endl;

	return true;
}

bool Exiter::execute(const string& command, LBA lba, DATA data, ISsdApp *app)
{
	cout << "Exit!!" << endl;
	return true;
}

