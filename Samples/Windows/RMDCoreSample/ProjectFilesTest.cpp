#include "stdafx.h"
#include <assert.h>

#include "ProjectFilesTest.h"
#include "UserTest.h"
#include "HttpReqest.h"

#include "rmccore/restful/rmuser.h"

using namespace RMCCORE;
using namespace std;

#define DEFAULT_PROJECTS_FILES		"DProjectFiles.txt"

const RMProjectFiles & GetProjectFiles(void)
{
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_PROJECTS_FILES);
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		throw;
	}

	static RMProjectFiles project;
	project.ImportFromJson(root->AsObject());

	return project;
}

void TestProjectFiles()
{
	RMUser user = GetDefaultUser();

	RMMyProjects *myproject = user.GetMyProjects();

	// check TestProject()

}
