#include "ssdApp.h"

DATA SsdApp::Read(LBA lba)
{
	// TODO: need to execute existing exe file of ssd
	return 0;
}

bool SsdApp::Write(LBA lba, DATA data)
{
	// TODO: need to execute existing exe file of ssd
	return true;
}

SsdApp* SsdApp::getInstance()
{
	static SsdApp instance;
	return &instance;
}
