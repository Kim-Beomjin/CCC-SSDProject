#pragma once
#include <string>

using DATA = unsigned int;
using LBA = unsigned int;

using std::string;

const string READ_CMD = "read";
const string WRITE_CMD = "write";
const string FULL_READ_CMD = "fullread";
const string FULL_WRITE_CMD = "fullwrite";
const string HELP_CMD = "help";
const string EXIT_CMD = "exit";

const LBA TEST_LBA = 2;
const DATA TEST_DATA = 0x12345678;
const DATA NO_DATA = 0x00000000;