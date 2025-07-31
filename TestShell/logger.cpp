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

void LogFile::SetName(const std::string& filename) {
    fileName = filename;
}

string LogFile::GetName(void) {
    return fileName;
}

string ZipUntilLogger::GetZipFileName(const std::string& old_filename) {
    string zipName = old_filename;
    const string target = ".log";
    const string replacement = ".zip";

    size_t pos = 0;
    while ((pos = zipName.find(target, pos)) != std::string::npos) {
        zipName.replace(pos, target.length(), replacement);
    }
    return zipName;
}

void ZipUntilLogger::ZipUntilLogFile(const std::string& old_filename) {
    string newName = GetZipFileName(old_filename);

    if (logFile.is_open())
        logFile.close();

    if (std::rename(old_filename.c_str(), newName.c_str()) != 0) {
        cout << "Rename Error" << endl;
        return;
    }

    SetName(newName);

    logFile.open(GetName(), ios::out | ios::app);
}

string UntilLogger::GetUntilFileName(void) {
    time_t now = time(nullptr);
    tm localTime{};

    localtime_s(&localTime, &now);  // Windows

    std::ostringstream oss;
    oss << "until_"
        << std::put_time(&localTime, "%Y%m%d_")
        << localTime.tm_hour << "h_"
        << localTime.tm_min << "m_"
        << localTime.tm_sec << "s.log";

    return oss.str();
}

void UntilLogger::SaveUntilLogger(const std::string& old_filename) {
    string newName = GetUntilFileName();

    if (logFile.is_open())
        logFile.close();
    /* Change oldUntilName.log -> newZipUntilName.log */
    zipUntilLogger.ZipUntilLogFile(GetName());

    /* Change latest.log -> newUntilName.log */
    if (std::rename(old_filename.c_str(), newName.c_str()) != 0) {
        cout << "Rename Error" << endl;
        return;
    }

    SetName(newName);

    logFile.open(GetName(), ios::out | ios::app);
}

void Logger::CheckManageUntilLogFile(void) {
    if (logFile.is_open() == false) {
        SetLogFile(LATEST_LOG_NAME);
    }

    if (GetLatestLogSize() > MANAGE_FILE_SIZE) {
        if (logFile.is_open())
            logFile.close();

        untilLogger.SaveUntilLogger(LATEST_LOG_NAME);

        SetLogFile(LATEST_LOG_NAME);
    }
}

int Logger::GetLatestLogSize(void) {
    return logFile.tellp();
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
