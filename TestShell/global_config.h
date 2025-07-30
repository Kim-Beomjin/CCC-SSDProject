#pragma once
#include <string>

#define FIX_ME_LATER (1)

using DATA = unsigned int;
using LBA = unsigned int;

using std::string;

const string READ_CMD = "read";
const string WRITE_CMD = "write";
const string SEND_READ_CMD = "R";
const string SEND_WRITE_CMD = "W";
const string FULL_READ_CMD = "fullread";
const string FULL_WRITE_CMD = "fullwrite";
const string HELP_CMD = "help";
const string EXIT_CMD = "exit";

const string SCRIPT_READ_CMD = "scriptread";
const string SCRIPT_WRITE_CMD = "scriptwrite";
const string FIRST_SCRIPT_SHORT_NAME = "1_";
const string FIRST_SCRIPT_FULL_NAME = "1_FullWriteAndReadCompare";
const string SECOND_SCRIPT_SHORT_NAME = "2_";
const string SECOND_SCRIPT_FULL_NAME = "2_PartialLBAWrite";
const string THIRD_SCRIPT_SHORT_NAME = "3_";
const string THIRD_SCRIPT_FULL_NAME = "3_WriteReadAging";

const LBA TEST_LBA = 2;
const DATA TEST_DATA = 0x12345678;
const DATA NO_DATA = 0x00000000;
