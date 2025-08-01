#pragma once
#include <string>

#define _FULL_TEST_RUN

#define FIX_ME_LATER (1)

using DATA = unsigned int;
using LBA = unsigned int;
using SIZE = unsigned int;

using std::string;

const string READ_CMD = "read";
const string WRITE_CMD = "write";
const string SEND_READ_CMD = "R";
const string SEND_WRITE_CMD = "W";
const string SEND_ERASE_CMD = "E";
const string SEND_FLUSH_CMD = "F";
const string FULL_READ_CMD = "fullread";
const string FULL_WRITE_CMD = "fullwrite";
const string HELP_CMD = "help";
const string EXIT_CMD = "exit";
const string ERASE_CMD = "erase";
const string ERASE_RANGE_CMD = "erase_range";
const string FLUSH_CMD = "flush";

const string FIRST_SCRIPT_SHORT_NAME = "1_";
const string FIRST_SCRIPT_FULL_NAME = "1_FullWriteAndReadCompare";
const string SECOND_SCRIPT_SHORT_NAME = "2_";
const string SECOND_SCRIPT_FULL_NAME = "2_PartialLBAWrite";
const string THIRD_SCRIPT_SHORT_NAME = "3_";
const string THIRD_SCRIPT_FULL_NAME = "3_WriteReadAging";
const string FOURTH_SCRIPT_SHORT_NAME = "4_";
const string FOURTH_SCRIPT_FULL_NAME = "4_EraseAndWriteAging";

const LBA TEST_LBA = 2;
const LBA TEST_START_LBA = 0;
const LBA TEST_START_LBA_VER_2 = 50;
const LBA TEST_END_LBA = 16;
const SIZE TEST_SIZE = 10;
const int TEST_MINUS_SIZE = -20;
const DATA TEST_DATA = 0x12345678;
const DATA NO_DATA = 0x00000000;

const int CMD_IDX = 0;
const int LBA_IDX = 1;
const int DATA_IDX = 2;

const int START_DATA_DIGIT_EXCEPT_HEX_PREFIX = 2;
const int DATA_NUM_DIGIT = 8;

const SIZE SSD_MAX_SIZE = 100;
const SIZE WRITE_DATA_SIZE = 10;
const SIZE SSD_ERASE_MAX_SIZE = 10;

const string EXE_FILE_NAME_RELEASE_PATH = "SSD.exe";
const string EXE_FILE_NAME_BUILD_PATH = "SSD.exe";