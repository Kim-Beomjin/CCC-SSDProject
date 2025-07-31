#pragma once
#include <iomanip>
#include <string>

using namespace std;

//#define LOG_DEBUG

#define SHELL_LOG(...) Logger::GetInstance().Log(__FUNCTION__, __VA_ARGS__)

class LogFile {
public:
    void SetLogFile(const std::string& filename);

    std::ofstream logFile;
private:

};

class Logger : public LogFile {
public:
    std::string GetCurrentTimeString(void);

    static Logger& GetInstance();

    template <typename... Args>
    void Log(const string& func, Args&&... args) {
        if (logFile.is_open() == false) {
            SetLogFile(LATEST_LOG_NAME);
        }
        LogImpl(func, std::forward<Args>(args)...);
    }
private:
    const string LATEST_LOG_NAME = "latest.log";

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
};

class UntilLogger : public LogFile {
public:
    void SaveUnillLogger(const std::string& message);

private:
};

class ZipUntilLogger : public LogFile {
public:
    void ZipUnillLogger(const std::string& message);

private:
};