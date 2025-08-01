#include "ssd.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

bool SSD::Read(LBA lba)
{
  if (IsLBAValidOrUpdateErrorToOutputFile(lba) == false)
  {
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
  if (IsLBAValidOrUpdateErrorToOutputFile(lba) == false)
  {
    return false;
  }

  TEST_READ_WRITE_LOGGER(lba, writeData);
  nand->Write(lba, writeData);
  return true;
}

bool SSD::Erase(LBA lba, ERASE_SIZE erase_size)
{
  if (IsLBAValidOrUpdateErrorToOutputFile(lba, erase_size) == false)
  {
    return false;
  }

  TEST_ERASE_LOGGER(lba, erase_size);
  return nand->Erase(lba, erase_size);
  return true;
}

void SSD::Flush()
{
    return;
}
bool SSD::IsLBAValidOrUpdateErrorToOutputFile(LBA lba, ERASE_SIZE erase_size)
{
  if (_IsInvalidParameter(lba, erase_size))
  {
    _UpdateOutputFile(ERROR_MSG);

    return false;
  }
  return true;
}

bool SSD::_IsInvalidParameter(LBA lba, ERASE_SIZE erase_size)
{
  if (lba >= LBA_END_ADDR || lba + erase_size > LBA_END_ADDR || erase_size > MAX_ERASE_SIZE)
  {
    TEST_LOGGER("INVALID PARAMETER");
    return true;
  }

  return false;
}

void SSD::UpdateOutputFileUsingData(const DATA data)
{
  _UpdateOutputFile(GlobalUtil::DataToHexString(data));
}

void SSD::_UpdateOutputFile(std::string stringData)
{
  std::string outputFile = OUTPUT_FILE;
  std::ofstream ofs(outputFile);

  ofs.write(stringData.c_str(), stringData.size());
}
