#pragma once
#include <string>

using LBA = unsigned int;
using DATA = unsigned int;

#define LBA_START_ADDR  (0)
#define LBA_END_ADDR    (100)
#define EMPTY_DATA      (0)
#define DATA_NUM_DIGIT  (8)

const std::string ERROR_MSG = "ERROR";
const std::string HEXA_PREFIX = "0x";
const char ZERO_PADDING = '0';
#ifdef _DEBUG
const std::string OUTPUT_FILE = "../x64/Debug/ssd_output.txt";
#else
const std::string OUTPUT_FILE = "../x64/Release/ssd_output.txt";
#endif

#ifdef _DEBUG
#define DEBUG_ASSERT(cond, msg)                     \
        do {                                        \
            if (!(cond)) {                          \
                throw std::runtime_error(msg);      \
            }                                       \
        } while (0)
#else
#define DEBUG_ASSERT(cond, msg) do { (void)(cond); (void)(msg); } while (0)
#endif