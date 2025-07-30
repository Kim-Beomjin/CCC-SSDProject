#include "commandParser.h"

bool CommandParser::ParseCommand(string cmd)
{
    if (IsVaildCommand(cmd) == false) return false;

    executor = ExecutorFactory().createExecutor(cmd);
    if (executor == nullptr) return false;

    return true;
}

bool  CommandParser::DoExecution(string cmd)
{
    bool ret = false;
    if (executor == nullptr) return false;

    ret = executor->execute(cmd);

    delete executor;
    return ret;
}

bool CommandParser::IsVaildCommand(string cmd)
{
    return true;
}
