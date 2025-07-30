#pragma once
#include "interface.h"

class Shell {
public:
    Shell() : commandParser(nullptr) {}
    Shell(ICommandParserBridge* broker) : commandParser(broker) {}

    void RunShellLoop(void);
#ifdef _DEBUG
    void setSsdApp(ISsdApp* app);
#endif

private:
    bool IsCmdExit(string cmd);
protected:
    ICommandParserBridge* commandParser = nullptr;
#ifdef _DEBUG
    ISsdApp* app = nullptr;
#endif
};
