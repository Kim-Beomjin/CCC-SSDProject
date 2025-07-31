#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "logger.h"

void LogFile::SetLogFile(const string& filename) {
    if (logFile.is_open())
        logFile.close();
    logFile.open(filename, ios::out | ios::app);
}

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

string Logger::GetCurrentTimeString() {
    time_t t = time(nullptr);
    tm local_tm{};

    localtime_s(&local_tm, &t);

    ostringstream oss;
    oss << "["
        << std::setfill('0') << std::setw(2) << (local_tm.tm_year % 100) << "."
        << std::setfill('0') << std::setw(2) << (local_tm.tm_mon + 1) << "."
        << std::setfill('0') << std::setw(2) << local_tm.tm_mday << " "
        << std::setfill('0') << std::setw(2) << local_tm.tm_hour << ":"
        << std::setfill('0') << std::setw(2) << local_tm.tm_min
        << "]";
    return oss.str();
}

string Logger::ReplaceDoubleColonWithDot(string str) {
    const string target = "::";
    const string replacement = ".";

    size_t pos = 0;
    while ((pos = str.find(target, pos)) != std::string::npos) {
        str.replace(pos, target.length(), replacement);
    }
    return str;
}

void UntilLogger::SaveUnillLogger(const std::string& filename) {
    if (logFile.is_open())
        logFile.close();
    logFile.open(filename, ios::out | ios::app);
}

void ZipUntilLogger::ZipUnillLogger(const std::string& filename) {
    if (logFile.is_open())
        logFile.close();
    logFile.open(filename, ios::out | ios::app);
}
