#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>

using namespace std;

//#define LOG_DEBUG

#ifdef _DEBUG
#define SHELL_LOG(...)
#else
#define SHELL_LOG(...) Logger::GetInstance().Log(__FUNCTION__, __VA_ARGS__)
#endif

class LogFile;

class ILogFileState {
public:
    virtual void SaveLog(LogFile& file, const std::string& message) = 0;
    virtual ~ILogFileState() = default;
};

class LogFile {
public:
    LogFile();

    void SaveLog(const std::string& message);

    void SetState(std::unique_ptr<ILogFileState> newState);

    void SetLogFile(const std::string& filename);

 //   void SetName(const std::string& filename);
 //   string GetName(void);
    void SetLatestName(const std::string& filename);
    string GetLatestName(void);

    void SetUntilName(const std::string& filename);
    string GetUntilName(void);

    //void SetFile(const std::string& filename);
    std::ofstream& GetFile(void);

private:
    string latestLogName = {};
    string untilLogName = {};
    string zipLogName = {};
    std::ofstream logFile;
    std::unique_ptr<ILogFileState> state;

};

class ZipUntilLogState : public ILogFileState {
public:
    void SaveLog(LogFile& logfile, const std::string& message) override;

private:
    void ZipUntilLogFile(const std::string& message);
    string MakeZipFileName(const std::string& old_filename);
};

class UntilLogState : public ILogFileState {
public:
    void SaveLog(LogFile& logfile, const std::string& message) override;

private:
    bool CheckChangeZip(LogFile& logfile);
    void SaveUntilLogger(LogFile& logfile, const std::string& old_filename);
    string MakeUntilFileName(void);
};

class LatestLogState : public ILogFileState {
public:
    void SaveLog(LogFile& logfile, const std::string& message) override;

private:
    const string LATEST_LOG_NAME = "latest.log";
    const int MANAGE_FILE_SIZE = 10 * 1024; // 10KB
    void WriteMessageToFile(ofstream& logfile, const std::string& message);
    bool CheckChangeUntil(ofstream& logfile);

    size_t GetLatestLogSize(ofstream& logFile);
};

class Logger {
public:
    std::string GetCurrentTimeString(void);

    static Logger& GetInstance();

    template <typename... Args>
    void Log(const string& func, Args&&... args) {
        string result;

        result = LogImpl(func, std::forward<Args>(args)...);

        logFile.SaveLog(result);
    }
private:
    template <typename T>
    void AppendToStream(std::ostringstream& oss, T&& value) {
        oss << std::forward<T>(value);
    }

    template <typename T, typename... Rest>
    void AppendToStream(std::ostringstream& oss, T&& first, Rest&&... rest) {
        oss << std::forward<T>(first) << " ";
        AppendToStream(oss, std::forward<Rest>(rest)...);
    }

    string ReplaceDoubleColonWithDot(string str);

    template <typename... Args>
    string LogImpl(const std::string& func, Args&&... args) {
        std::ostringstream oss;
        string message;
        string fix_func = ReplaceDoubleColonWithDot(func + "()");

        oss << GetCurrentTimeString() << " ";
        oss << std::left << std::setw(30) << fix_func << " : ";
        AppendToStream(oss, std::forward<Args>(args)...);
        oss << std::endl;

        return oss.str();
    }
    LogFile logFile;
};