#pragma once
#include "ssd.h"

enum ARGV
{
    PGM_IDX = 0,
    CMD_IDX,
    LBA_IDX,
    DATA_IDX,
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
    bool _LoadWriteParameterAndCheckInvalid(char* lbaStr, char* dataStr);
    bool _LoadReadParameterAndCheckInvalid(char* lbaStr);
    bool _IsNegative(char* lbaStr);
    bool _IsInvalidLength(char* dataStr);
    unsigned int _SafeStoul(char* str);
    LBA lba;
    DATA data;
    SSD* ssd;

    const int WRITE_COMMAND_ARG_COUNT = 4;
    const int READ_COMMAND_ARG_COUNT = 3;
    const char* WRITE_CMD = "W";
    const char* READ_CMD = "R";
    const char ZERO = '0';
    const char SMALL_EX = 'x';
    const char LARGE_EX = 'X';
};