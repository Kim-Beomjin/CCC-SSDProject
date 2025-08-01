#pragma once

#include "global_config.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#ifdef _DEBUG
#include "gmock/gmock.h"
#endif

class GlobalUtil
{
public:
  static std::string DataToHexString(const DATA data)
  {
    std::stringstream hexString;
    hexString << HEXA_PREFIX
      << std::setw(DATA_NUM_DIGIT) << std::setfill(ZERO)
      << std::hex << std::uppercase
      << data;
    std::string hexStringData = hexString.str();

    return hexStringData;
  }

  static bool IsNegative(char* lbaStr)
  {
    return (std::string(lbaStr)[0] == '-');
  }

  static bool IsInvalidLength(char* dataStr)
  {
    return (std::string(dataStr).size() != 10 || std::string(dataStr)[0] != ZERO || !(std::string(dataStr)[1] == LARGE_EX || std::string(dataStr)[1] == SMALL_EX));
  }

  static unsigned int SafeStoul(char* str, int base)
  {
    size_t idx;
    unsigned int ret = std::stoul(std::string(str), &idx, base);
    if (idx != std::string(str).size())
    {
      throw(std::exception("INVALID INPUT PARAMETERS"));
    }
    return ret;
  }

  static std::vector<std::string> Split(const std::string& str, const char delimiter)
  {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
      result.push_back(item);
    }
    return result;
  }

#ifdef _DEBUG
  static void DBG_RemoveOutputFile(void)
  {
    int result = std::remove(OUTPUT_FILE.c_str());
    if (result != 0)
    {
      std::perror("file remove fail");
    }
  }

  static std::string DBG_GetDataFromOutputFile(void)
  {
    std::ifstream ifs(OUTPUT_FILE);
    std::string readData((std::istreambuf_iterator<char>(ifs)),
      std::istreambuf_iterator<char>());

    return readData;
  }

  static void ValidateOutputDataWith(const std::string expectedData)
  {
    std::string outputData = GlobalUtil::DBG_GetDataFromOutputFile();
    EXPECT_EQ(expectedData, outputData);
  }
#endif
};