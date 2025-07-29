#include <iostream>
#include <string>

using namespace std;

class CommandParser {
public:
    bool ParseCommend(string cmd) {
        if (IsVaildCommend(cmd) == false)
            return false;

        return true;
    }
private:
    bool IsVaildCommend(string cmd) {
        return true;
    }
};