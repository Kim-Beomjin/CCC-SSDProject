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
    HostInterface() : ssd{ new SSD() }, bufferManager{ new BufferManager(ssd) } {};
    SSD* ssd;
    BufferManager* bufferManager;
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
    IProcessor* CreateProcessor(int argc, char* argv[], SSD* ssd, BufferManager* bufferManager);
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
    WriteProcessor(SSD* ssd, BufferManager* bufferManager) : 
        ssd{ ssd }, bufferManager{ bufferManager }, lba{ 0 }, data{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char* dataStr);
    void Process();
private:
    LBA lba;
    DATA data;
    HostInterfaceUtil hostUtil;
    BufferManager* bufferManager;
    SSD* ssd;
};

class ReadProcessor : public IProcessor
{
public:
    ReadProcessor(SSD* ssd, BufferManager* bufferManager) : 
        ssd{ ssd }, bufferManager{ bufferManager }, lba{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char*);
    void Process();
private:
    LBA lba;
    HostInterfaceUtil hostUtil;
    BufferManager* bufferManager;
    SSD* ssd;
};

class EraseProcessor : public IProcessor
{
public:
    EraseProcessor(SSD* ssd, BufferManager* bufferManager) :
        ssd{ ssd }, bufferManager{ bufferManager }, lba{ 0 }, size{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char* sizeStr);
    void Process();
private:
    LBA lba;
    unsigned int size;
    HostInterfaceUtil hostUtil;
    BufferManager* bufferManager;
    SSD* ssd;
};

class FlushProcessor : public IProcessor
{
public:
    FlushProcessor(SSD* ssd, BufferManager* bufferManager) :
        ssd{ ssd }, bufferManager{ bufferManager } {}
    bool LoadParameterAndCheckInvalid(char*, char*);
    void Process();
private:
    HostInterfaceUtil hostUtil;
    BufferManager* bufferManager;
    SSD* ssd;
};