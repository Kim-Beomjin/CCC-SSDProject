#include <iostream>
#include <string>

using namespace std;

class CommandParser {
public:
    bool ParseCommend(string cmd) {
        if (IsVaildCommand(cmd) == false)
            return false;

        return true;
    }
private:
    bool IsVaildCommand(string cmd) {
        return true;
    }
};
