#include <fstream>
#include <string>
#include "shell.h"
#include "ssdApp.h"
#include "commandParser.h"
#include "executorFactory.h"
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
        commandParser = make_shared<CommandParser>(static_pointer_cast<IExecutorFactory>(make_shared<StrategyCompositeExecutorFactory>()));

    string cmd;
    while (std::getline(infile, cmd)) {
        if (cmd.empty())
        {
            return 0; // Empty Line
        }

        printWithoutLineBreak(cmd + "  ___  ");
        if (!commandParser->ParseCommand(cmd))
        {
            print("FAIL!");
            return 0; // Invalid command
        }

        printWithoutLineBreak("Run()...");
        commandParser->ExecuteSsdUsingParsedCommand(SsdApp::getInstance());
    }

    return 0;
}