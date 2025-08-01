#include <iostream>
#include "utils.h"
#include "commandParser.h"

bool CommandParser::ParseCommand(const string& fullCmd) {
    std::vector<string> tokens = splitCommand(fullCmd);
    if (tokens.empty()) return false;

    if (doParse(tokens) == false) {
        cout << "INVALID COMMAND" << endl;
        return false;
    }

    executor = ExecutorFactory().createExecutor(command);
    if (executor == nullptr) {
        return false;
    }

    if (false == executor->IsValidCommand(tokens)) return false;

    return true;
}

bool CommandParser::ExecuteSsdUsingParsedCommand(ISsdApp* app) {
    if (app == nullptr) return false;
    return executor->execute(app, lba, data);
}

bool CommandParser::IsVaildCommand(const string& cmd) {
    if (cmd_set.find(cmd) != cmd_set.end()) return true;
    return false;
}

bool CommandParser::doParse(const vector<string>& tokens) {
    command = tokens[CMD_IDX];
    if (IsVaildCommand(command) == false) return false;

    if (tokens.size() > LBA_IDX) {
        if (setLbaFromToken(tokens[LBA_IDX]) == false) return false;
    }
    
    if (tokens.size() > DATA_IDX) {
        if (setDataFromToken(tokens[DATA_IDX]) == false) return false;
    }

    return true;
}

bool CommandParser::setLbaFromToken(const string& strLba) {
    try {
        lba = stringToUnsignedInt(strLba);
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
        return false;
    }

    return true;
}

bool CommandParser::setDataFromToken(const string& strData) {
    try {
        data = stringToUnsignedInt(strData);
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
        return false;
    }

    return true;
}
