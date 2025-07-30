#pragma once
using LBA = unsigned int;
using DATA = unsigned int;

#define LBA_START_ADDR  (0)
#define LBA_END_ADDR    (100)
#define EMPTY_DATA      (0)
const std::string ERROR_MSG = "ERROR";

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