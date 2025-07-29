#include <gmock/gmock.h>
#include <sstream>
#include "requestsender.cpp"

TEST(RequestSenderTest, ReceiveCommandTest) {
	RequestSender sender;
	string program_name = "mock_ssd";
	string command = "W";
	unsigned int addr = 2;
	unsigned int data = 0xAAAABBBB;

	sender.ReceiveCommand(program_name, command, addr, data);
	EXPECT_EQ(sender.GetProgramName(), program_name);
	EXPECT_EQ(sender.GetCommand(), command);
	EXPECT_EQ(sender.GetAddress(), addr);
	EXPECT_EQ(sender.GetData(), data);
}

TEST(RequestSenderTest, ReceiveCommandWithInvalidCommandTest) {
	RequestSender sender;
	string program_name = "mock_ssd";
	string command = "HI";
	unsigned int addr = 2;
	unsigned int data = 0xAAAABBBB;

	std::streambuf* original_buf = std::cout.rdbuf();
	std::ostringstream capture_output;
	std::cout.rdbuf(capture_output.rdbuf());

	sender.ReceiveCommand(program_name, command, addr, data);

	std::cout.rdbuf(original_buf);

	std::string output = capture_output.str();
	EXPECT_NE(output.find("INVALID_COMMAND"), std::string::npos);
}

TEST(RequestSenderTest, SendRequestTest) {
	RequestSender sender;
	string program_name = "mock_ssd";
	string command = "W";
	unsigned int addr = 2;
	unsigned int data = 0xAAAABBBB;

	sender.ReceiveCommand(program_name, command, addr, data);

	int ret = sender.SendRequest();
	EXPECT_EQ(ret, 0); // Assuming 0 is the success code
}
