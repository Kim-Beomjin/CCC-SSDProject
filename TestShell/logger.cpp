#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "logger.h"

LogFile::LogFile() {
    SetState(std::make_unique<LatestLogState>());
}

void LogFile::SaveLog(const std::string& message) {
    state->SaveLog(*this, message);

    SetState(std::make_unique<LatestLogState>());
}

void LogFile::SetState(std::unique_ptr<ILogFileState> newState) {
    state = std::move(newState);
}

void LogFile::SetLogFile(const string& filename) {
    if (logFile.is_open())
        logFile.close();
    logFile.open(filename, ios::out | ios::app);
    SetLatestName(filename);
}

void LogFile::SetLatestName(const std::string& filename) {
    latestLogName = filename;
}

string LogFile::GetLatestName(void) {
    return latestLogName;
}

void LogFile::SetUntilName(const std::string& filename) {
    untilLogName = filename;
}

string LogFile::GetUntilName(void) {
    return untilLogName;
}

ofstream& LogFile::GetFile(void) {
    return logFile;
}

string ZipUntilLogState::MakeZipFileName(const std::string& old_filename) {
    string zipName = old_filename;
    const string target = ".log";
    const string replacement = ".zip";

    size_t pos = 0;
    while ((pos = zipName.find(target, pos)) != std::string::npos) {
        zipName.replace(pos, target.length(), replacement);
    }
    return zipName;
}

void ZipUntilLogState::ZipUntilLogFile(const std::string& oldName) {
    string newName = MakeZipFileName(oldName);

    //cout << __FUNCTION__ << " " << oldName << "->" << newName << endl;

    if (std::rename(oldName.c_str(), newName.c_str()) != 0) {
        cout << "Rename Error" << endl;
        return;
    }
}

void ZipUntilLogState::SaveLog(LogFile& logfile, const std::string& message)
{
    ofstream& logFile = logfile.GetFile();

    if (logFile.is_open())
        logFile.close();

    ZipUntilLogFile(logfile.GetUntilName());
}

string UntilLogState::MakeUntilFileName(void) {
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

void UntilLogState::SaveUntilLogger(LogFile& logfile, const std::string& newName) {
    string oldName = logfile.GetLatestName();

    //cout << __FUNCTION__ << " " << oldName << "->" << newName << endl;

    if (std::rename(oldName.c_str(), newName.c_str()) != 0) {
        cout << "Rename Error" << endl;
        return;
    }

    logfile.SetUntilName(newName);
}

void UntilLogState::SaveLog(LogFile& logfile, const std::string& message)
{
    ofstream& logFile = logfile.GetFile();
    bool needStateChange = 0;

    if (logFile.is_open())
        logFile.close();

    needStateChange = CheckChangeZip(logfile);
    if (needStateChange) {
        /* Change oldUntilName.log -> newZipUntilName.log */
        logfile.SetState(std::make_unique<ZipUntilLogState>());

        logfile.SaveLog(message);
    }

    /* Change latest.log -> newUntilName.log */
    string newname = MakeUntilFileName();
    SaveUntilLogger(logfile, newname);
}

bool UntilLogState::CheckChangeZip(LogFile& logfile) {
    string name = logfile.GetUntilName();


    if (name.empty())
        return false;

    return true;
}

void LatestLogState::SaveLog(LogFile& logfile, const std::string& message)
{
    ofstream& logFile = logfile.GetFile();
    bool needStateChange = 0;

    logfile.SetLogFile(LATEST_LOG_NAME);

    WriteMessageToFile(logFile, message);

    needStateChange = CheckChangeUntil(logFile);
    if (needStateChange) {
        logfile.SetState(std::make_unique<UntilLogState>());

        logfile.SaveLog(message);
    }
}

void LatestLogState::WriteMessageToFile(ofstream& logfile, const std::string& message)
{
    logfile << message;
}

bool LatestLogState::CheckChangeUntil(ofstream& logfile) {
    if (GetLatestLogSize(logfile) > MANAGE_FILE_SIZE) {
        return true;
    }

    return false;
}

int LatestLogState::GetLatestLogSize(ofstream& logFile) {
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
