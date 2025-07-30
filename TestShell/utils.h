#pragma once
#include "global_config.h"
#include <vector>

using std::vector;

const string makeExecuteCmd(string cmd, LBA lba = 0, DATA data = 0);
string DataToHexString(const DATA data);
unsigned int stringToUnsignedInt(const string& str);
vector<string> splitCommand(const string& fullCmd);