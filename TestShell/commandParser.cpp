#include <iostream>
#include <string>
#include "interface.h"

using namespace std;

class ICommandParserBridge {
public:
    virtual bool ParseCommand(string cmd) = 0;
};

class CommandParser : public ICommandParserBridge {
public:
    bool ParseCommand(string cmd) override {
        if (IsVaildCommand(cmd) == false)
            return false;

		executor = ExecutorFactory().createExecutor(cmd);
        if (executor == nullptr) {
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
