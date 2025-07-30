#include "ssd.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

bool SSD::Read(LBA lba)
{
  if (_IsInvalidParameter(lba))
  {
    _UpdateOutputFile(ERROR_MSG);

    return false;
   }

  DATA readData;
  if (nand->Read(lba, readData) == false)
  {
    return false;
  }

  _UpdateOutputFile(_DataToHexString(readData));

  return true;
}

bool SSD::Write(LBA lba, DATA writeData)
{
  if (_IsInvalidParameter(lba))
  {
    _UpdateOutputFile(ERROR_MSG);

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

void SSD::_UpdateOutputFile(std::string stringData)
{
  std::string outputFile = OUTPUT_FILE;
  std::ofstream ofs(outputFile);

  ofs.write(stringData.c_str(), stringData.size());
}

std::string SSD::_DataToHexString(const DATA data)
{
  std::stringstream hexString;
  hexString << HEXA_PREFIX
  << std::setw(DATA_NUM_DIGIT) << std::setfill(ZERO_PADDING)
  << std::hex << std::uppercase
  << data;
  std::string hexStringData = hexString.str();

  return hexStringData;
}