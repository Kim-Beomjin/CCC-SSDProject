#pragma once
#include "global_config.h"
#include "interface.h"

class SsdApp : public ISsdApp
{
public:
	DATA Read(LBA lba) override;

	bool Write(LBA lba, DATA data) override;

	static SsdApp* getInstance();

private:
	SsdApp() {}
	const string makeExecuteCmd(string cmd, LBA lba = 0, DATA data = 0);
	string _DataToHexString(const DATA data);

	const int DATA_NUM_DIGIT = 8;
	const string EXE_FILE_NAME = "..\\x64\\Release\\SSD.exe";
};
