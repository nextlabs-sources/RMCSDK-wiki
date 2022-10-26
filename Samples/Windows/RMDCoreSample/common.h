#pragma once

#include "rmccore/restful/rmsyspara.h"

const std::string ReadFromFile(std::string filename);

bool WriteToFile(std::string filename, std::string content);

const std::string GenerateClientID(void);

const RMCCORE::RMSystemPara &GenerateSystemParameter(void);