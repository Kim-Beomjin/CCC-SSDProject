#pragma once

#include "nand.h"

#include <iostream>
#include <fstream>
#include <string>

class SSD
{
public:
  SSD(NandInterface* nand) : nand {nand}{};
  bool read(int lba);
  bool write(int lba, int data);

private:
  NandInterface* nand;
};