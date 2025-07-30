#include <iostream>
#include <direct.h>
#include "ssdApp.h"
#include "utils.h"

DATA SsdApp::Read(LBA lba) {
	string sendReadString = makeExecuteCmd(SEND_READ_CMD, lba);
	system(sendReadString.c_str());
	return 0;
}

bool SsdApp::Write(LBA lba, DATA data) {
	string sendWriteString = makeExecuteCmd(SEND_WRITE_CMD, lba, data);
	system(sendWriteString.c_str());
	return true;
}

SsdApp* SsdApp::getInstance()
{
	static SsdApp instance;
	return &instance;
}

