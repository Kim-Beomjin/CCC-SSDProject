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
  TEST_READ_WRITE_LOGGER(lba, readData);

  UpdateOutputFileUsingData(readData);

  return true;
}

bool SSD::Write(LBA lba, DATA writeData)
{
  if (_IsInvalidParameter(lba))
  {
    _UpdateOutputFile(ERROR_MSG);

    return false;
  }

  TEST_READ_WRITE_LOGGER(lba, writeData);
  return nand->Write(lba, writeData);
}

bool SSD::Erase(LBA lba, ERASE_SIZE erase_size)
{
  if (_IsInvalidParameter(lba, erase_size))
  {
    _UpdateOutputFile(ERROR_MSG);

    return false;
  }

  TEST_ERASE_LOGGER(lba, erase_size);
  return nand->Erase(lba, erase_size);
}

bool SSD::_IsInvalidParameter(LBA lba, ERASE_SIZE erase_size)
{
  if (lba >= LBA_END_ADDR || lba + erase_size > LBA_END_ADDR || erase_size > 10)
  {
    TEST_LOGGER("INVALID PARAMETER");
    return true;
  }

  return false;
}

void SSD::UpdateOutputFileUsingData(const DATA data)
{
  _UpdateOutputFile(_DataToHexString(data));
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