#include "utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::endl;

bool fileExists(const std::string& filename) {
	std::ifstream file(filename);
	return file.is_open();
}

const string makeExecuteCmd(string cmd, LBA lba, DATA data) {
	string appFile = EXE_FILE_NAME_RELEASE_PATH;

	if (!fileExists(EXE_FILE_NAME_RELEASE_PATH))
	{
		appFile = EXE_FILE_NAME_BUILD_PATH;
	}

	std::ostringstream oss;
	oss << appFile << " " << cmd;

	if (cmd == SEND_FLUSH_CMD) return oss.str();
	
	oss << " " << lba;
	if (cmd == SEND_WRITE_CMD) {
		oss << " " << DataToHexString(data);
	}
	else if (cmd == SEND_ERASE_CMD) {
		oss << " " << data;
	}
//#ifdef _DEBUG
	cout << oss.str() << endl;
//#endif
	return oss.str();
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
			throw std::invalid_argument("문자열 중간에 숫자 이외의 문자가 있음");
		}
		if (value > std::numeric_limits<unsigned int>::max()) {
			throw std::out_of_range("값이 unsigned int 범위를 초과함");
		}

		return static_cast<unsigned int>(value);
	}
	catch (const std::exception& e) {
		std::cerr << "변환 실패: " << e.what() << std::endl;
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
