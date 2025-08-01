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
        // 분기 ~~~.txt면 Runner();
        // 기존 커맨드거나 숫자로 시작하면 기존 내용으로
        // cin.ignore();
        // Parse cmd
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
