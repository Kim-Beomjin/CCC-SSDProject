#include <iostream>
#include "utils.h"
#include "commandParser.h"

bool CommandParser::ParseCommand(const string& fullCmd) {
    if (doParse(fullCmd) == false) {
        cout << "INVALID COMMAND" << endl;
        return false;
    }

    executor = ExecutorFactory().createExecutor(command);
    if (executor == nullptr) {
        return false;
    }

    return true;
}

bool CommandParser::ExecuteSsdUsingParsedCommand(ISsdApp* app) {
    if (app == nullptr) return false;
    return executor->execute(command, lba, data, app);
}

bool CommandParser::IsVaildCommand(const string& cmd, size_t tokenSize) {
    if (cmd_set.find(cmd) != cmd_set.end()) {
        if (cmd == WRITE_CMD && tokenSize <= 2) return false;
        if (cmd == READ_CMD && tokenSize > 2) return false;
        return true;
    }
    return false;
}

bool CommandParser::doParse(const string& fullCmd) {
    std::vector<string> tokens = splitCommand(fullCmd);

    if (tokens.empty()) return false;

    command = tokens[0];
    if (IsVaildCommand(command, tokens.size()) == false) return false;
    if (tokens.size() > 1) {
        try {
            lba = stringToUnsignedInt(tokens[1]);
        }
        catch (const std::exception& e) {
            cout << e.what() << endl;
            return false;
        }
    }
    
    if (tokens.size() > 2) {
        try {
            data = stringToUnsignedInt(tokens[2]);
        }
        catch (const std::exception& e) {
            cout << e.what() << endl;
            return false;
        }
    }

    return true;
}


