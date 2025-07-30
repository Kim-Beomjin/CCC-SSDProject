#pragma once

#include "nand.h"
#include "global_config.h"
#include <iostream>
#include <fstream>
#include <string>

class SSD
{
public:
  SSD(NandInterface* nand) : nand {nand}{};
  bool read(LBA lba);
  bool write(LBA lba, DATA data);

private:
  NandInterface* nand;
};