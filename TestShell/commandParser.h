#pragma once
#include <unordered_set>
#include "global_config.h"
#include "interface.h"

using namespace std;

class CommandParser : public ICommandParserBridge {
public:
    CommandParser() = default;
    CommandParser(IExecutorFactory* factory) : _factory{ factory } {}
    bool ParseCommand(const string& cmd) override;
    bool ExecuteSsdUsingParsedCommand(ISsdApp* app) override;
private:
    bool doParse(const vector<string>& tokens);

    bool setLbaFromToken(const string& strLba);
    bool setDataFromToken(const string& strData);
    bool IsLbaRangeValid();

    IExecutorFactory* _factory;
    IExecutor* executor;
    string command = "";
    LBA lba = 0;
    DATA data = -1;
};
