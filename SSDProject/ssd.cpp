#include "ssd.h"

bool SSD::read(int lba)
{
  if (lba > 99)
  {
#ifdef _DEBUG
    throw(std::exception("INVALID PARAMETER"));
#endif
    return false;
  }

  bool result;
  int data;
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

bool SSD::write(int lba, int data)
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