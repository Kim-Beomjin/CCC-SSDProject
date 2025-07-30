#pragma once

#include "nand.h"
#include "global_config.h"

class SSD
{
public:
  SSD(NandInterface* nand) : nand {nand}{};
  bool read(LBA lba);
  bool write(LBA lba, DATA writeData);

private:
  bool _IsInvalidParameter(LBA lba);
  void _updateOutputFile(std::string data);
  NandInterface* nand;
};