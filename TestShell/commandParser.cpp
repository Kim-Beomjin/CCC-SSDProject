#include "utils.h"
#include "commandParser.h"

bool CommandParser::ParseCommand(const string& fullCmd) {
    std::vector<string> tokens = splitCommand(fullCmd);
    if (tokens.empty()) return false;

    if (doParse(tokens) == false) {
        print("INVALID COMMAND");
        return false;
    }

    executor = ExecutorFactory().createExecutor(command);
    if (executor == nullptr) {
        print("INVALID COMMAND");
        return false;
    }

    if (false == executor->IsValidCommand(tokens)) return false;

    return true;
}

bool CommandParser::ExecuteSsdUsingParsedCommand(ISsdApp* app) {
    if (app == nullptr) return false;
    return executor->execute(app, lba, data);
}

bool CommandParser::doParse(const vector<string>& tokens) {
    command = tokens[CMD_IDX];
    if (tokens.size() > LBA_IDX) {
        if (setLbaFromToken(tokens[LBA_IDX]) == false) return false;
        if (IsLbaRangeValid() == false) {
            print("LBA is out of range - LBA" + std::to_string(lba));
            return false;
        }
    }
    
    if (tokens.size() > DATA_IDX) {
        if (setDataFromToken(tokens[DATA_IDX]) == false) return false;
    }

    return true;
}

bool CommandParser::setLbaFromToken(const string& strLba) {
    try {
        lba = stringToUnsignedInt(strLba);
        return true;
    }
    catch (const std::exception& e) {
        print(e.what());
        return false;
    }
}

bool CommandParser::setDataFromToken(const string& strData) {
    try {
        data = stringToUnsignedInt(strData);
        return true;
    }
    catch (const std::exception& e) {
        print(e.what());
        return false;
    }
}

bool CommandParser::IsLbaRangeValid() {
    if (lba >= 0 && lba < static_cast<LBA>(SSD_MAX_SIZE)) return true;
    return false;
}
