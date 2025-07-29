#include <iostream>
#include <string>

using namespace std;

class Shell {
public:
    void LoopGetCommand(void) {
        string cmd;
        do {
            cout << "Shell> ";

            cin.ignore();

            getline(std::cin, cmd);

            // Parse cmd

        } while (IsCmdExit(cmd) == false);
    }

    bool IsCmdExit(string cmd) {
        return cmd == "exit";
    }
};