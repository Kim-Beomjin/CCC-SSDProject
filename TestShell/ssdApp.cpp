#include <direct.h>
#include <exception>
#include "ssdApp.h"
#include "utils.h"

bool SsdApp::Read(LBA lba) {
	try {
		string sendReadString = TestShellUtils::makeExecuteCmd(SEND_READ_CMD, lba);
		system(sendReadString.c_str());
	}
	catch (std::exception& e) {
		print(e.what());
	}
	return 0;
}

bool SsdApp::Write(LBA lba, DATA data) {
	try {
		string sendWriteString = TestShellUtils::makeExecuteCmd(SEND_WRITE_CMD, lba, data);
		system(sendWriteString.c_str());
	}
	catch (std::exception& e) {
		print(e.what());
	}
	return true;
}

bool SsdApp::Erase(LBA lba, SIZE size) {
	try {
		string sendEraseString = TestShellUtils::makeExecuteCmd(SEND_ERASE_CMD, lba, size);
		system(sendEraseString.c_str());
	}
	catch (std::exception& e) {
		print(e.what());
	}
	return true;
}

bool SsdApp::Flush() {
	try {
		string sendFlushString = TestShellUtils::makeExecuteCmd(SEND_FLUSH_CMD);
		system(sendFlushString.c_str());
	}
	catch (std::exception& e) {
		print(e.what());
	}
	return true;
}

SsdApp* SsdApp::getInstance()
{
	static SsdApp instance;
	return &instance;
}

