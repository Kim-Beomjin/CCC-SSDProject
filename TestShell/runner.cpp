#include <iostream>
#include <fstream>
#include <string>
#include "shell.h"
#include "ssdApp.h"
#include "commandParser.h"

using namespace std;

int Shell::Runner(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "[Runner] Invalid Param\n";
        return 0; // Invalid Param
    }

    const string& filename = argv[1];
    ifstream infile(filename);
    if (!infile.is_open())
    {
        cout << "[Runner] Open Fail\n";
        return 0; // Open Fail
    }

    if (commandParser == nullptr)
        commandParser = new CommandParser();

    string cmd;
    while (std::getline(infile, cmd)) {
        if (cmd.empty())
        {
            cout << "[Runner] Empty Line\n";
            return 0; // Empty Line
        }

        if (!commandParser->ParseCommand(cmd))
        {
            cout << "[Runner] Invalid Command\n";
            return 0; // Invalid command
        }

        commandParser->ExecuteSsdUsingParsedCommand(SsdApp::getInstance());
    }

    return 0;
}