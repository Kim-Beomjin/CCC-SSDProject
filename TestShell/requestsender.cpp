#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class RequestSender {
public:
	RequestSender() : program_name_(""), command_(""), addr_(0), data_(0) {}
	void ReceiveCommand(const string& program_name, const string& command, unsigned int addr, unsigned int data) {
		program_name_ = program_name;
		command_ = command;
		addr_ = addr;
		data_ = data;
		if (command != "W" && command != "R") {
			cout << "INVALID_COMMAND: " << command << endl;
		}
	}

	int SendRequest() {
		return 0;
	}

	const string& GetProgramName() const {
		return program_name_;
	}

	const string& GetCommand() const {
		return command_;
	}

	unsigned int GetAddress() const {
		return addr_;
	}

	unsigned int GetData() const {
		return data_;
	}
private:
	string program_name_;
	string command_;
	unsigned int addr_;
	unsigned int data_;
};
