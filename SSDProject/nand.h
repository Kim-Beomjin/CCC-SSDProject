#pragma once
#include <string>
#include <fstream>
#define NAND_FILE_NAME ("ssd_nand.txt")

class NandInterface
{
	virtual bool Read(const int lba, int &out) = 0;
	virtual bool Write(const int lba, const int data) = 0;
}

{
public:
	bool Read (const int lba, int &out) override;
	bool Write(const int lba, const int data) override;
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
	bool _IsFileExists(const std::string& filename);
	void _CreateFile(const std::string& filename);
	const int LBA_START_ADDR = 0;
	const int LBA_END_ADDR = 100;};