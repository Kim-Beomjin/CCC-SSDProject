#include "utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;

const string makeExecuteCmd(string cmd, LBA lba, DATA data) {
	std::ostringstream oss;
	oss << EXE_FILE_NAME << " " << cmd << " " << lba;
	if (cmd == SEND_WRITE_CMD) {
		oss << " " << DataToHexString(data);
	}
	string commandStr = oss.str();
	cout << commandStr << endl;
	return commandStr;
}

string DataToHexString(const DATA data)
{
	std::stringstream hexString;
	hexString << "0x"
		<< std::setw(DATA_NUM_DIGIT) << std::setfill('0')
		<< std::hex << std::uppercase
		<< data;
	std::string hexStringData = hexString.str();

	return hexStringData;
}

unsigned int stringToUnsignedInt(const string& str) {
	try {
		size_t pos = 0;
		unsigned long value = std::stoul(str, &pos, 0);

		if (pos != str.size()) {
			throw std::invalid_argument("���ڿ� �߰��� ���� �̿��� ���ڰ� ����");
		}
		if (value > std::numeric_limits<unsigned int>::max()) {
			throw std::out_of_range("���� unsigned int ������ �ʰ���");
		}

		return static_cast<unsigned int>(value);
	}
	catch (const std::exception& e) {
		std::cerr << "��ȯ ����: " << e.what() << std::endl;
		throw std::runtime_error(e.what());
	}
}

vector<string> splitCommand(const string& fullCmd) {
	vector<string> tokens = {};
	std::istringstream iss(fullCmd);
	std::string token;
	while (iss >> token) {
		tokens.push_back(token);
	}
	return tokens;
}
