#include <iostream>
#include <string>

using namespace std;

class CommandParser {
public:
    bool ParseCommand(string cmd) {
        if (IsVaildCommand(cmd) == false)
            return false;

        return true;
    }
private:
    bool IsVaildCommand(string cmd) {
        return true;
    }
};
