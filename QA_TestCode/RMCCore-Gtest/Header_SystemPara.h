#pragma once

RMSystemPara GenerateSysPara();
const RMSystemPara CreateDefaultSysPara(void);

const RMSystemPara &GenerateSystemParameter(void);
const RMSystemPara &GenerateSystemParameter(string DeviceID);
const RMSystemPara &GenerateSystemParameter(RMProduct product, RMPlatformID PlatformID, RMProcessorArch processor);