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
    return executor->execute(app, lba, data);
}

bool CommandParser::IsVaildCommand(const string& cmd, size_t tokenSize) {
    if (cmd_set.find(cmd) != cmd_set.end()) {
        if (IsWriteCmdValid(cmd, tokenSize)) return true;
        else if (IsReadCmdValid(cmd, tokenSize)) return true;
        else if (tokenSize == 1) return true;
        return false;
    }
    return false;
}

bool CommandParser::IsWriteCmdValid(const string& cmd, size_t tokenSize) {
    if (cmd == WRITE_CMD && tokenSize == 3) return true;
    return false;
}

bool CommandParser::IsReadCmdValid(const string& cmd, size_t tokenSize) {
    if (cmd == READ_CMD && tokenSize == 2) return true;
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
        if (command == WRITE_CMD) {
            if (IsValidWriteData(tokens[2]) == false) return false;
        }

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

bool CommandParser::IsValidWriteData(const string& data) {
    if (data.size() != 10) return false;

    if (!(data[0] == '0' && (data[1] == 'x' || data[1] == 'X')))
        return false;

    for (size_t i = 2; i < 10; ++i) {
        if (!std::isxdigit(static_cast<unsigned char>(data[i])))
            return false;
    }

    return true;
}

