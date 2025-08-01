#pragma once

#include "nand.h"
#include "global_config.h"
#include "global_util.h"

class SSD
{
public:
  SSD() : nand{ new Nand() } {};
  SSD(NandInterface* nand) : nand {nand}{};
  bool Read(LBA lba);
  bool Write(LBA lba, DATA writeData);
  bool Erase(LBA lba, ERASE_SIZE erase_size);
  void UpdateOutputFileUsingData(const DATA data);

private:
  bool _IsInvalidParameter(LBA lba, ERASE_SIZE erase_size = 0);
  void _UpdateOutputFile(std::string stringData);
  NandInterface* nand;
};