#pragma once
#include <iomanip>
#include <string>

using namespace std;

//#define LOG_DEBUG

#ifdef _DEBUG
#define SHELL_LOG(...)
#else
#define SHELL_LOG(...) Logger::GetInstance().Log(__FUNCTION__, __VA_ARGS__)
#endif

class LogFile {
public:
    void SetLogFile(const std::string& filename);

    std::ofstream logFile;

    void SetName(const std::string& filename);
    string GetName(void);
private:
    string fileName;

};

class ZipUntilLogger : public LogFile {
public:
    void ZipUntilLogFile(const std::string& message);

private:
    string GetZipFileName(const std::string& old_filename);
};

class UntilLogger : public LogFile {
public:
    void SaveUntilLogger(const std::string& old_filename);

private:
    string GetUntilFileName(void);

    ZipUntilLogger zipUntilLogger;
};

class Logger : public LogFile {
public:
    std::string GetCurrentTimeString(void);

    static Logger& GetInstance();

    template <typename... Args>
    void Log(const string& func, Args&&... args) {
        CheckManageUntilLogFile();

        LogImpl(func, std::forward<Args>(args)...);
    }
private:
    const string LATEST_LOG_NAME = "latest.log";
    const int MANAGE_FILE_SIZE = 10 * 1024; // 10KB

    void CheckManageUntilLogFile();

    int GetLatestLogSize();

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
    void LogImpl(const std::string& func, Args&&... args) {
        std::ostringstream oss;
        string message;
        string fix_func = ReplaceDoubleColonWithDot(func + "()");

        oss << GetCurrentTimeString() << " ";
        oss << std::left << std::setw(30) << fix_func << " : ";
        AppendToStream(oss, std::forward<Args>(args)...);

#ifdef LOG_DEBUG
        cout << oss.str() << std::endl;
#else
        logFile << oss.str() << std::endl;
#endif
    }
    UntilLogger untilLogger;
};