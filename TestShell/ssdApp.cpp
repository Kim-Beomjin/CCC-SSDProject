#include <sstream>
#include <iostream>
#include <direct.h>
#include "ssdApp.h"

using std::cout;
using std::endl;

DATA SsdApp::Read(LBA lba) {
	system(makeExecuteCmd(SEND_READ_CMD, lba));
	return 0;
}

bool SsdApp::Write(LBA lba, DATA data) {
	system(makeExecuteCmd(SEND_WRITE_CMD, lba, data));
	return true;
}

SsdApp* SsdApp::getInstance()
{
	static SsdApp instance;
	return &instance;
}

const char* SsdApp::makeExecuteCmd(string cmd, LBA lba, DATA data) {
	std::ostringstream oss;
	oss << EXE_FILE_NAME << " " << cmd << " " << lba;
	if (data >= 0) {
		oss << " 0x" << std::hex << std::uppercase << data;
	}
	string commandStr = oss.str();
	cout << commandStr << endl;
	return commandStr.c_str();
}