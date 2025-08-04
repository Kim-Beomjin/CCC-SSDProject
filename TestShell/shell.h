#pragma once
#include "interface.h"

class Shell {
public:
    Shell() = default;
    Shell(shared_ptr<ICommandParserBridge> parser) : commandParser{ parser } {}

    int RunShellLoop(void);
    int Runner(int argc, char* argv[]);
#ifdef _DEBUG
    void setSsdApp(ISsdApp* app);
#endif

private:
    bool IsCmdExit(string cmd);
protected:
    shared_ptr<ICommandParserBridge> commandParser;
#ifdef _DEBUG
    ISsdApp* app = nullptr;
#endif
};
