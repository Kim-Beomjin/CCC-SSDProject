#pragma once
#include <unordered_set>
#include "global_config.h"
#include "interface.h"

using namespace std;

class CommandParser : public ICommandParserBridge {
public:
    bool ParseCommand(const string& cmd) override;
    bool ExecuteSsdUsingParsedCommand(ISsdApp* app) override;
private:
    bool doParse(const vector<string>& tokens);

    bool setLbaFromToken(const string& strLba);
    bool setDataFromToken(const string& strData);

    IExecutor* executor;
    string command = "";
    LBA lba = 0;
    DATA data = -1;

    std::unordered_set<string> cmd_set = {
        READ_CMD, FULL_READ_CMD,
        WRITE_CMD,FULL_WRITE_CMD,
        ERASE_CMD, ERASE_RANGE_CMD,
        HELP_CMD, EXIT_CMD, FLUSH_CMD,
        FIRST_SCRIPT_SHORT_NAME, SECOND_SCRIPT_SHORT_NAME, THIRD_SCRIPT_SHORT_NAME, FOURTH_SCRIPT_SHORT_NAME,
        FIRST_SCRIPT_FULL_NAME, SECOND_SCRIPT_FULL_NAME, THIRD_SCRIPT_FULL_NAME, FOURTH_SCRIPT_FULL_NAME,
    };
};
