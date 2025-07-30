#include "ssd.h"

bool SSD::read(LBA lba)
{
  if (_IsInvalidParameter(lba))
  {
    _updateOutputFile(ERROR_MSG);

    return false;
   }

  DATA readData;
  if (nand->Read(lba, readData) == false)
  {
    return false;
  }
  
  _updateOutputFile(std::to_string(readData));

  return true;
}

bool SSD::write(LBA lba, DATA writeData)
{
  if (_IsInvalidParameter(lba))
  {
    _updateOutputFile(ERROR_MSG);

    return false;
  }

  return nand->Write(lba, writeData);
}

bool SSD::_IsInvalidParameter(LBA lba)
{
  if (lba >= LBA_END_ADDR)
  {
    return true;
  }

  return false;
}

void SSD::_updateOutputFile(std::string stringData)
{
  std::string outputFile = "ssd_output.txt";
  std::ofstream ofs(outputFile);

  ofs.write(stringData.c_str(), stringData.size());
}
