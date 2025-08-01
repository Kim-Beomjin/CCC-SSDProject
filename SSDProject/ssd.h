#pragma once
#include "interface.h"
#include "nand.h"
#include "global_config.h"
#include "global_util.h"

class SSD : public ISSD
{
public:
  SSD() : nand{ new Nand() } {};
  SSD(NandInterface* nand) : nand {nand}{};
  bool Read(LBA lba) override;
  bool Write(LBA lba, DATA writeData) override;
  bool Erase(LBA lba, ERASE_SIZE erase_size) override;
  void Flush() override;

protected:
  bool IsLBAValidOrUpdateErrorToOutputFile(LBA lba, ERASE_SIZE erase_size = 0);
  void UpdateOutputFileUsingData(const DATA data);

private:
  bool _IsInvalidParameter(LBA lba, ERASE_SIZE erase_size = 0);
  void _UpdateOutputFile(std::string stringData);
  NandInterface* nand;
};