#pragma once

#include <string>

#ifdef  UNICODE 
typedef  std::wstring TString;
#else
typedef std::string TString;
#endif