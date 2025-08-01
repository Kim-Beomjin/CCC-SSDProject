#pragma once
#include "global_config.h"
#include <vector>

using std::vector;

class TestShellUtils {
public:
	static bool fileExists(const std::string& filename);
	static const string makeExecuteCmd(string cmd, LBA lba = 0, DATA data = 0);
	static string DataToHexString(const DATA data);
	static unsigned int stringToUnsignedInt(const string& str);
	static vector<string> splitCommand(const string& fullCmd);
};

void print(const string& desc);
void dbgPring(const string& desc);