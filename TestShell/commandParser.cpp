#include <iostream>
#include <string>
#include "executor.cpp"

using namespace std;

class CommandParser {
public:
    bool ParseCommand(string cmd) {
        if (IsVaildCommand(cmd) == false)
            return false;

		executor = ExecutorFactory().createExecutor(cmd);
        if (executor == nullptr) {
            cout << "Command not found: " << cmd << endl;
            return false;
		}
        return true;
    }
private:
    bool IsVaildCommand(string cmd) {
        return true;
    }

    IExecutor* executor;
};
