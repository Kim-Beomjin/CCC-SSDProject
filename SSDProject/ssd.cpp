#include "ssd.h"

bool SSD::read(LBA lba)
{
  if (_IsInvalidParameter(lba))
  {
    return false;
  }

  DATA data;
  if (nand->Read(lba, data) == false)
  {
    return false;
  }
  
  _updateOutputFile(data);

  return true;
}

bool SSD::write(LBA lba, DATA data)
{
  if (_IsInvalidParameter(lba))
  {
    return false;
  }

  return nand->Write(lba, data);
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

void SSD::_updateOutputFile(DATA data)
{
  std::string outputFile = "ssd_output.txt";
  std::ofstream ofs(outputFile);

  std::string stringData = std::to_string(data);
  ofs.write(stringData.c_str(), stringData.size());
  ofs.close();
}
