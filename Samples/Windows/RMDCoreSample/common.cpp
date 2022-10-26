#include "stdafx.h"
#include "common.h"

#include "rmccore\rmccore.h"
#include "rmccore\utils\string.h"

using namespace RMCCORE;

const std::string  ReadFromFile(std::string filename)
{
	std::string retstr;

	FILE * testfile;
	fopen_s(&testfile, filename.c_str(), "r");
	if (testfile == NULL) {
		return retstr;
	}

	char buffer[257];
	while (!feof(testfile)) {
		memset(buffer, 0, sizeof(buffer));
		size_t s = fread(buffer, sizeof(char), sizeof(buffer) - 1, testfile);
		retstr += buffer;
	}
	fclose(testfile);

	return retstr;
}

bool WriteToFile(std::string filename, std::string content)
{
	FILE * testfile;
	fopen_s(&testfile, filename.c_str(), "w+");
	if (testfile == NULL) {
		return false;
	}

	fwrite(content.c_str(), sizeof(char), content.length(), testfile);
	fclose(testfile);

	return true;

}
const std::string GenerateClientID(void)
{
	std::string clientid;
	for (int i = 0; i < 16; i++) {
		unsigned char r = (unsigned char)std::rand();
		std::string tmp = utohs<char>(r);
		clientid += tmp;
	}

	return clientid;
}

const RMSystemPara &GenerateSystemParameter(void)
{
	static RMSystemPara para;
	para.SetDeviceID("Testing app");

	return para;
}