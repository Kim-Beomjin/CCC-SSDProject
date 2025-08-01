#pragma once
#include "ssd.h"
#include "BufferManager.h"
#include "Interface.h"
enum ARGV
{
    PGM_IDX = 0,
    CMD_IDX,
    LBA_IDX,
    DATA_IDX = 3,
    SIZE_IDX = 3,
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
    HostInterface() :bufferedSSD{ new BufferedSSD() } {};
    BufferedSSD* bufferedSSD;
};

class HostInterfaceUtil
{
public:
    bool IsNegative(char* lbaStr)
    {
        return (std::string(lbaStr)[0] == '-');
    }
    bool IsInvalidLength(char* dataStr)
    {

        return (std::string(dataStr).size() != 10 || std::string(dataStr)[0] != ZERO || !(std::string(dataStr)[1] == LARGE_EX || std::string(dataStr)[1] == SMALL_EX));
    }

    unsigned int SafeStoul(char* str, int base)
    {
        size_t idx;
        unsigned int ret = std::stoul(std::string(str), &idx, base);
        if (idx != std::string(str).size()) {
            throw(std::exception("INVALID INPUT PARAMETERS"));
        }
        return ret;
    }
private:
    const char ZERO = '0';
    const char SMALL_EX = 'x';
    const char LARGE_EX = 'X';
};

class ProcessorFactory
{
public:
    static ProcessorFactory* GetInstance() {
        static ProcessorFactory processorFactory;
        return &processorFactory;
    }
    IProcessor* CreateProcessor(int argc, char* argv[], BufferedSSD* bufferedSSD);
private:
    ProcessorFactory() {};
    bool _WriteCondition(int argc, char* argv[]);
    bool _ReadCondition(int argc, char* argv[]);
    bool _EraseCondition(int argc, char* argv[]);
    bool _FlushCondition(int argc, char* argv[]);

    const int WRITE_COMMAND_ARG_COUNT = 4;
    const int READ_COMMAND_ARG_COUNT = 3;
    const int ERASE_COMMAND_ARG_COUNT = 4;
    const int FLUSH_COMMAND_ARG_COUNT = 2;

    const char* WRITE_CMD = "W";
    const char* READ_CMD = "R";
    const char* ERASE_CMD = "E";
    const char* FLUSH_CMD = "F";

};

class WriteProcessor : public IProcessor
{
public:
    WriteProcessor(BufferedSSD* bufferedSSD) : 
        bufferedSSD{ bufferedSSD }, lba{ 0 }, data{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char* dataStr) override;
    void Process() override;
private:
    LBA lba;
    DATA data;
    GlobalUtil hostUtil;
    BufferedSSD* bufferedSSD;
};

class ReadProcessor : public IProcessor
{
public:
    ReadProcessor(BufferedSSD* bufferedSSD) : 
        bufferedSSD{ bufferedSSD }, lba{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char*) override;
    void Process() override;
private:
    LBA lba;
    HostInterfaceUtil hostUtil;
    BufferedSSD* bufferedSSD;
};

class EraseProcessor : public IProcessor
{
public:
    EraseProcessor(BufferedSSD* bufferedSSD) :
        bufferedSSD{ bufferedSSD }, lba{ 0 }, size{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char* sizeStr) override;
    void Process() override;
private:
    LBA lba;
    unsigned int size;
    HostInterfaceUtil hostUtil;
    BufferedSSD* bufferedSSD;
};

class FlushProcessor : public IProcessor
{
public:
    FlushProcessor(BufferedSSD* bufferedSSD) :
        bufferedSSD{ bufferedSSD } {}
    bool LoadParameterAndCheckInvalid(char*, char*) override;
    void Process() override;
private:
    HostInterfaceUtil hostUtil;
    BufferedSSD* bufferedSSD;
};