#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "global_config.h"
#include "interface.h"

using namespace std;

class CommandParser : public ICommandParserBridge {
public:
    bool ParseCommand(const string& cmd) override;
    bool ExecuteSsdUsingParsedCommand(ISsdApp* app) override;

private:
    bool IsVaildCommand(const string& cmd);
    bool IsValidLba(const LBA lba);
    bool doParse(const string& cmd);
    std::vector<string> splitCommand(const string& cmd);
    unsigned int stringToUnsignedInt(const string& str);

    IExecutor* executor;
    string command = "";
    LBA lba = 0;
    DATA data = -1;

    std::unordered_set<string> cmd_set = { "read", "write", "fullread", "fullwrite", "help", "exit" };
};
