#include "ssd.h"

bool SSD::read(LBA lba)
{
  if (lba > 99)
  {
#ifdef _DEBUG
    throw(std::exception("INVALID PARAMETER"));
#endif
    return false;
  }

  bool result;
  DATA data;
  std::string outputFile = "ssd_output.txt";

  result = nand->Read(lba, data);
  if (result)
  {
    std::ofstream ofs(outputFile);
    
    std::string stringData = std::to_string(data);
    ofs.write(stringData.c_str(), stringData.size());
    ofs.close();
  }

  return result;
}

bool SSD::write(LBA lba, DATA data)
{
  if (lba > 99)
  {
#ifdef _DEBUG
    throw(std::exception("INVALID PARAMETER"));
#endif
    return false;
  }

  return nand->Write(lba, data);
}