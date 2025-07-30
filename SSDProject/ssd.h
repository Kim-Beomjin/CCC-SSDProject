#pragma once

#include "nand.h"
#include "global_config.h"

class SSD
{
public:
  SSD() : nand{ new Nand() } {};
  SSD(NandInterface* nand) : nand {nand}{};
  bool Read(LBA lba);
  bool Write(LBA lba, DATA writeData);

private:
  bool _IsInvalidParameter(LBA lba);
  void _UpdateOutputFile(std::string data);
  std::string _DataToHexString(const DATA data);
  NandInterface* nand;
};