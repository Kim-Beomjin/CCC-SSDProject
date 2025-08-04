#pragma once
#include <string>

using LBA = unsigned int;
using DATA = unsigned int;
using ERASE_SIZE = unsigned int;

const int LBA_START_ADDR = 0;
const int LBA_END_ADDR = 100;
const int EMPTY_DATA = 0;
const int DATA_NUM_DIGIT = 8;
const int MAX_ERASE_SIZE = 10;
const int DECIMAL_BASE = 10;
const int HEXA_BASE = 16;
const std::string ERROR_MSG = "ERROR";
const std::string HEXA_PREFIX = "0x";
const char ZERO = '0';
const char SMALL_EX = 'x';
const char LARGE_EX = 'X';

const std::string OUTPUT_FILE = "ssd_output.txt";

#ifdef _TEST_CONFIG
#include <iostream>

#define TEST_LOGGER(msg) \
    std::cout << "[LOG][" << __func__ << "] " << msg << std::endl;
#else
#define TEST_LOGGER(msg) \
    ;
#endif

#define TEST_READ_WRITE_LOGGER(lba, data) \
    TEST_LOGGER("LBA : " << lba << ", Data : " << data);
#define TEST_ERASE_LOGGER(lba, size) \
    TEST_LOGGER("LBA : " << lba << ", Size : " << size);

#ifdef _DEBUG
#define DEBUG_ASSERT(cond, msg)                     \
        do {                                        \
            if (!(cond)) {                          \
                throw std::runtime_error(msg);      \
            }                                       \
        } while (0)
#elif _TEST_CONFIG
#define DEBUG_ASSERT(cond, msg) \
    TEST_LOGGER(msg)
#else
#define DEBUG_ASSERT(cond, msg) do { (void)(cond); (void)(msg); } while (0)
#endif
