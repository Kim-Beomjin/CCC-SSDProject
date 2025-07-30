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
	const char* makeExecuteCmd(string cmd, LBA lba = 0, DATA data = 0);
	const string EXE_FILE_NAME = "SSD.exe";
};
