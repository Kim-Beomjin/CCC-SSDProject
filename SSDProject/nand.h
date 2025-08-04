#pragma once

#include "global_config.h"
#include "global_util.h"

#define NAND_FILE_NAME ("./ssd_nand.txt")

class NandInterface
{
public:
	virtual bool Read(const LBA lba, DATA& readData) = 0;
	virtual bool Write(const LBA lba, const DATA writeData) = 0;
	virtual bool Erase(const LBA lba, const ERASE_SIZE erase_size) = 0;
};

class Nand : public NandInterface
{
public:
  Nand() {}
  bool Read(const LBA lba, DATA& readData) override;
  bool Write(const LBA lba, const DATA writeData) override;
  bool Erase(const LBA lba, const ERASE_SIZE erase_size) override;
private:
	bool IsInvalidParameter(const LBA lba, const ERASE_SIZE erase_size = 0);
	
	void _LoadNandFromFile(const std::string& filename);
	void _DumpNandToFile(const std::string& filename);


	bool _IsInvalidFile(const std::string& filename);
	void _CreateFile(const std::string& filename);
	void _ReadFile(const std::string& filename);
	static const unsigned long long FILE_SIZE = (LBA_END_ADDR - LBA_START_ADDR) * sizeof(int);
	int nandData[LBA_END_ADDR - LBA_START_ADDR] = { 0 };
	
};