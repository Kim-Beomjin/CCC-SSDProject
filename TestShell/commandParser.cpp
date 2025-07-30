#include <iostream>
#include <string>
#include "interface.h"

using namespace std;

class CommandParserBridge {
public:
    virtual bool ParseCommand(string cmd) = 0;
};

class CommandParser : public CommandParserBridge {
public:
    bool ParseCommand(string cmd) override {
        if (IsVaildCommand(cmd) == false)
            return false;

		executor = ExecutorFactory().createExecutor(cmd);
        if (executor == nullptr) {
            //cout << "Command not found: " << cmd << endl;
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
