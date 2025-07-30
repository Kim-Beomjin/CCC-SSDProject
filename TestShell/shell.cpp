#include <iostream>
#include <string>

using namespace std;

class Shell {
public:
    void RunShellLoop(void) {
        string cmd;
        do {
            cout << "Shell> ";



            getline(std::cin, cmd);
            // cin.ignore();
            // Parse cmd
#ifdef _DEBUG
            cout << cmd << "\n";
#endif
        } while (IsCmdExit(cmd) == false);
    }

    bool IsCmdExit(string cmd) {
        return cmd == "exit";
    }
};