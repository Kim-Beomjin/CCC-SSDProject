#pragma once
#include <string>
#include "interface.h"

using namespace std;

class ICommandParserBridge
{
public:
    virtual bool ParseCommand(string cmd) = 0;
    virtual bool DoExecution(string cmd) = 0;
};

class CommandParser : public ICommandParserBridge {
public:
    bool ParseCommand(string cmd) override;
    bool DoExecution(string cmd) override;
private:
    bool IsVaildCommand(string cmd);

    IExecutor* executor;
};
