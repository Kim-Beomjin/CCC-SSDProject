#pragma once
#include <string>
#include <fstream>
#define NAND_FILE_NAME ("./ssd_nand.txt")

class NandInterface
{
public:
	virtual bool Read(const int lba, int& out) = 0;
	virtual bool Write(const int lba, const int data) = 0;
};

class Nand : public NandInterface
{
public:
  Nand() {}
  bool Read(const int lba, int& out);
  bool Write(const int lba, const int data);
private:
	bool _IsValidParameter(const int lba);
	bool _IsFileValid(const std::string& filename);
	void _CreateFile(const std::string& filename);
	void _ReadFile(const std::string& filename);
	void _WriteFile(const std::string& filename);
	static const int LBA_START_ADDR = 0;
	static const int LBA_END_ADDR = 100;
	static const unsigned long long FILE_SIZE = (LBA_END_ADDR - LBA_START_ADDR) * sizeof(int);
	const int EMPTY_DATA = 0;
	int nandData[LBA_END_ADDR - LBA_START_ADDR];
	
};