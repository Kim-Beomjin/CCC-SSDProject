#include <iostream>
#include <sstream>
#include <stdexcept>
#include "commandParser.h"

bool CommandParser::ParseCommand(const string& fullCmd) {
    if (doParse(fullCmd) == false) return false;

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

bool CommandParser::IsVaildCommand(const string& cmd) {
    if (cmd_set.find(cmd) != cmd_set.end()) return true;
    return false;
}

bool CommandParser::IsValidLba(const LBA lba) {
    if (lba >= SSD_MAX_SIZE) return false;
    return true;
}

bool CommandParser::doParse(const string& fullCmd) {
    std::vector<string> tokens = splitCommand(fullCmd);

    if (tokens.empty()) return false;

    command = tokens[0];
    if (IsVaildCommand(command) == false) return false;
    if (tokens.size() > 1) {
        lba = stringToUnsignedInt(tokens[1]);
        if (IsValidLba(lba) == false) return false;
    }
    if (tokens.size() > 2) data = stringToUnsignedInt(tokens[2]);

    return true;
}

std::vector<string> CommandParser::splitCommand(const string& fullCmd) {
    std::vector<string> tokens = {};
    std::istringstream iss(fullCmd);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

unsigned int CommandParser::stringToUnsignedInt(const string& str) {
    try {
        size_t pos = 0;
        unsigned long value = std::stoul(str, &pos, 0);

        if (pos != str.size()) {
            throw std::invalid_argument("문자열 중간에 숫자 이외의 문자가 있음");
        }
        if (value > std::numeric_limits<unsigned int>::max()) {
            throw std::out_of_range("값이 unsigned int 범위를 초과함");
        }

        return static_cast<unsigned int>(value);
    }
    catch (const std::exception& e) {
        std::cerr << "변환 실패: " << e.what() << std::endl;
        throw std::runtime_error(e.what());
    }
}
