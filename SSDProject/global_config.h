#pragma once
#include <string>

using LBA = unsigned int;
using DATA = unsigned int;

#define LBA_START_ADDR  (0)
#define LBA_END_ADDR    (100)
#define EMPTY_DATA      (0)
#define DATA_NUM_DIGIT  (8)
#define MAX_ERASE_SIZE  (10)

const std::string ERROR_MSG = "ERROR";
const std::string HEXA_PREFIX = "0x";
const char ZERO_PADDING = '0';

const std::string OUTPUT_FILE = "ssd_output.txt";

#ifdef _TEST_CONFIG
#include <iostream>

#define TEST_LOGGER(msg) \
    std::cout << "[LOG] " << __func__ << ": " << msg << std::endl;
#else
#define TEST_LOGGER(msg) \
    ;
#endif

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
