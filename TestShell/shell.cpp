#include <iostream>
#include "shell.h"
#include "ssdApp.h"
#include "commandParser.h"

using namespace std;

void Shell::RunShellLoop(void) {
    string cmd;

    if (commandParser == nullptr)
        commandParser = new CommandParser();

    do {
        cout << "Shell> ";

        getline(std::cin, cmd);
        bool ret = commandParser->ParseCommand(cmd);
#ifdef _DEBUG
        cout << cmd << "\n";
#endif
#ifdef _DEBUG
        if (ret)
            commandParser->ExecuteSsdUsingParsedCommand(app);
#else
        if (ret)
            commandParser->ExecuteSsdUsingParsedCommand(SsdApp::getInstance());
#endif


    } while (IsCmdExit(cmd) == false);
}

bool Shell::IsCmdExit(string cmd) {
    return cmd == EXIT_CMD;
}

#ifdef _DEBUG
void Shell::setSsdApp(ISsdApp* pApp) {
    app = pApp;
}

#endif
