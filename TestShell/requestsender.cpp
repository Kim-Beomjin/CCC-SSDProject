#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class RequestSender {
public:
	RequestSender() : program_name_(""), command_(""), addr_(0), data_(0) {}
	void ReceiveCommand(const string& program_name, const string& command, unsigned int addr, unsigned int data) {

	}

	int SendRequest() {
		return 0;
	}
private:
	string program_name_;
	string command_;
	unsigned int addr_;
	unsigned int data_;
};
