#include <fstream>
#include <string>
#include "shell.h"
#include "ssdApp.h"
#include "commandParser.h"
#include "utils.h"

using namespace std;

int Shell::Runner(int argc, char* argv[])
{
    if (argc != 2)
    {
        print("[Runner] Invalid Param");
        return 0; // Invalid Param
    }

    const string& filename = argv[1];
    ifstream infile(filename);
    if (!infile.is_open())
    {
        print("[Runner] Open Fail");
        return 0; // Open Fail
    }

    if (commandParser == nullptr)
        commandParser = new CommandParser();

    string cmd;
    while (std::getline(infile, cmd)) {
        if (cmd.empty())
        {
            print("[Runner] Empty Line");
            return 0; // Empty Line
        }

        printWithoutLineBreak(cmd + "  ___  ");
        if (!commandParser->ParseCommand(cmd))
        {
            print("[Runner] Invalid Command");
            return 0; // Invalid command
        }

        printWithoutLineBreak("Run()...");
        commandParser->ExecuteSsdUsingParsedCommand(SsdApp::getInstance());
    }

    return 0;
}