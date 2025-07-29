#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "commandParser.cpp"

TEST(ExecutorTest, exitCommandTest) {
	CommandParser parser;

	parser.ParseCommand("help");

}

TEST(ExecutorTest, writeCommandTest) {
	CommandParser parser;

	parser.ParseCommand("write");

	parser.ParseCommand("read");
}

TEST(ExecutorTest, readCommandTest) {
	CommandParser parser;

	parser.ParseCommand("read");
}

#endif