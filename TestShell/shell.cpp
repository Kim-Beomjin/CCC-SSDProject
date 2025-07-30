#include <iostream>
#include <string>
#include "commandParser.h"

using namespace std;

class Shell {
public:
    Shell() : commandParser(nullptr) {}
    Shell(ICommandParserBridge* broker) : commandParser(broker) {}

    void RunShellLoop(void) {
        string cmd;

        if (commandParser == nullptr)
            commandParser = new CommandParser();

        do {
            cout << "Shell> ";

            getline(std::cin, cmd);
            // cin.ignore();
            // Parse cmd
            bool ret = commandParser->ParseCommand(cmd);

#ifdef _DEBUG
            cout << cmd << "\n";
#endif
        } while (IsCmdExit(cmd) == false);
    }

private:
    bool IsCmdExit(string cmd) {
        return cmd == "exit";
    }

protected:
    ICommandParserBridge *commandParser = nullptr;
};
