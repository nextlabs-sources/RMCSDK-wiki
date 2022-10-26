#include "RMCCORE\restful\rmtenant.h"
#include "RMCCORE\restful\rmuser.h"

using namespace RMCCORE;
using namespace std;

const std::string ReadFromFile(std::string filename)
{
	std::string retstr;

	FILE * testfile;
	fopen_s(&testfile, filename.c_str(), "r");
	if (testfile == NULL) {
		return retstr;
	}

	char buffer[257];
	//memset(buffer, 0, sizeof(buffer));
	while (!feof(testfile)) {
		memset(buffer, 0, sizeof(buffer));
		fread(buffer, sizeof(char), sizeof(buffer) - 1, testfile);
		retstr += buffer;
	}
	fclose(testfile);

	return retstr;
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



 