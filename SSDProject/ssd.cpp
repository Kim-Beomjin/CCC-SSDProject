#include "ssd.h"

bool SSD::read(LBA lba)
{
  if (_IsInvalidParameter(lba))
  {
    return false;
  }

  DATA readData;
  if (nand->Read(lba, readData) == false)
  {
    return false;
  }
  
  _updateOutputFile(readData);

  return true;
}

bool SSD::write(LBA lba, DATA writeData)
{
  if (_IsInvalidParameter(lba))
  {
    return false;
  }

  return nand->Write(lba, writeData);
}

bool SSD::_IsInvalidParameter(LBA lba)
{
  if (lba >= LBA_END_ADDR)
  {
#ifdef _DEBUG
    throw(std::exception("INVALID PARAMETER"));
#endif
    return true;
  }

  return false;
}

void SSD::_updateOutputFile(DATA Data)
{
  std::string outputFile = "ssd_output.txt";
  std::ofstream ofs(outputFile);

  std::string stringData = std::to_string(Data);
  ofs.write(stringData.c_str(), stringData.size());
  ofs.close();
}
