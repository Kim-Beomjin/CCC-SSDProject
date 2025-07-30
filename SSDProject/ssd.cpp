#include "ssd.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

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

  std::stringstream ss;
  ss << "0x"
    << std::setw(8) << std::setfill('0')  // 8자리 0패딩
    << std::hex << std::uppercase         // 대문자 16진수
    << readData;
  std::string hexStr = ss.str();
  _updateOutputFile(hexStr);

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
