#pragma once

#include "global_config.h"

#ifdef _DEBUG
#include <fstream>
#include <sstream>
#include <iomanip>
#endif

class GlobalUtil
{
public:
#ifdef _DEBUG
  static std::string DBG_GetDataFromOutputFile(void)
  {
    std::ifstream ifs(OUTPUT_FILE);
    std::string readData((std::istreambuf_iterator<char>(ifs)),
      std::istreambuf_iterator<char>());

    return readData;
  }

  static void DBG_RemoveOutputFile(void)
  {
    int result = std::remove(OUTPUT_FILE.c_str());
    if (result != 0)
    {
      std::perror("file remove fail");
    }
  }
#endif
};