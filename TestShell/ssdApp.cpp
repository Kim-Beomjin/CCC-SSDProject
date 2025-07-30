#include <sstream>
#include <iostream>
#include "ssdApp.h"

using std::cout;
using std::endl;

DATA SsdApp::Read(LBA lba) {
	system(makeExecuteCmd(READ_CMD, lba));
	return 0;
}

bool SsdApp::Write(LBA lba, DATA data) {
	system(makeExecuteCmd(WRITE_CMD, lba, data));
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
		oss << " " << std::hex << std::uppercase << data;
	}
	string commandStr = oss.str();
	cout << commandStr << endl;
	return commandStr.c_str();
}