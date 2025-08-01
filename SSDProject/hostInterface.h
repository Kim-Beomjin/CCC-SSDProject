#pragma once
#include "ssd.h"
#include "BufferedSSD.h"
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
    void SetSSD(ISSD* ssd)
    {
        this->ssd = ssd;
    }
#endif

private:
    HostInterface() :ssd{ new BufferedSSD() } {};
    ISSD* ssd;
};

class ProcessorFactory
{
public:
    static ProcessorFactory* GetInstance() {
        static ProcessorFactory processorFactory;
        return &processorFactory;
    }
    IProcessor* CreateProcessor(int argc, char* argv[], ISSD* ssd);
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
    WriteProcessor(ISSD* ssd) : 
        ssd{ ssd }, lba{ 0 }, data{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char* dataStr) override;
    bool Process() override;
private:
    LBA lba;
    DATA data;
    ISSD* ssd;
};

class ReadProcessor : public IProcessor
{
public:
    ReadProcessor(ISSD* ssd) : 
        ssd{ ssd }, lba{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char*) override;
    bool Process() override;
private:
    LBA lba;
    ISSD* ssd;
};

class EraseProcessor : public IProcessor
{
public:
    EraseProcessor(ISSD* ssd) :
        ssd{ ssd }, lba{ 0 }, size{ 0 } {}
    bool LoadParameterAndCheckInvalid(char* lbaStr, char* sizeStr) override;
    bool Process() override;
private:
    LBA lba;
    unsigned int size;
    ISSD* ssd;
};

class FlushProcessor : public IProcessor
{
public:
    FlushProcessor(ISSD* ssd) :
        ssd{ ssd } {}
    bool LoadParameterAndCheckInvalid(char*, char*) override;
    bool Process() override;
private:
    ISSD* ssd;
};