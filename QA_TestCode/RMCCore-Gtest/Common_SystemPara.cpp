#include "stdafx.h"

const RMSystemPara &GenerateSystemParameter(string DeviceID)
{
	static RMSystemPara para;
	//para.SetDeviceID("Testing app");
	para.SetDeviceID(DeviceID);

	return para;
}

const RMSystemPara &GenerateSystemParameter(void)
{
	static RMSystemPara para;
	para.SetDeviceID("Testing app");

	return para;
}

const RMSystemPara &GenerateSystemParameter(RMProduct product, RMPlatformID PlatformID, RMProcessorArch processor)
{
	static RMSystemPara para;
	para.SetPlatformID(PlatformID);
	para.SetProduct(product);
	para.SetProcessor(processor);

	return para;
}