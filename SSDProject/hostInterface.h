#pragma once
#include "ssd.h"

enum ARGV
{
    PGM_IDX = 0,
    CMD_IDX,
    LBA_IDX,
    DATA_IDX = 3,
    LENGTH_IDX = 3,
};

class HostInterface
{
public:
    static HostInterface* GetInstance() {
        static HostInterface hostinterface; 
        return &hostinterface;
    }

    void Execute(int argc, char* argv[]);

#if _DEBUG
    void SetSSD(SSD* ssd)
    {
        this->ssd = ssd;
    }
#endif

private:
    HostInterface() : ssd{ new SSD() }, lba{ 0 }, data{ 0 } {};
    bool _WriteCondition(int argc, char* argv[]);
    bool _ReadCondition(int argc, char* argv[]);
    bool _EraseCondition(int argc, char* argv[]);
    bool _FlushCondition(int argc, char* argv[]); //TODO Become Interface

    bool _LoadWriteParameterAndCheckInvalid(char* lbaStr, char* dataStr);
    bool _LoadReadParameterAndCheckInvalid(char* lbaStr, char*);
    bool _LoadEraseParameterAndCheckInvalid(char* lbaStr, char* lengthStr);
    bool _LoadFlushParameterAndCheckInvalid(char*, char*); //TODO Become Interface

    bool _IsNegative(char* lbaStr);
    bool _IsInvalidLength(char* dataStr);
    unsigned int _SafeStoul(char* str, int base);
    LBA lba;
    DATA data;
    SSD* ssd;

    const int WRITE_COMMAND_ARG_COUNT = 4;
    const int READ_COMMAND_ARG_COUNT = 3;
    const int ERASE_COMMAND_ARG_COUNT = 4;
    const int FLUSH_COMMAND_ARG_COUNT = 2;
    const char* WRITE_CMD = "W";
    const char* READ_CMD = "R";
    const char* ERASE_CMD = "E";
    const char* FLUSH_CMD = "F";
    const char ZERO = '0';
    const char SMALL_EX = 'x';
    const char LARGE_EX = 'X';
};