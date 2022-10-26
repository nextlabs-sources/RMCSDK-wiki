#include "stdafx.h"
#include <assert.h>

#include "ProjectTest.h"
#include "UserTest.h"
#include "HttpReqest.h"

#include "rmccore/restful/rmuser.h"
#include "rmccore/restful/rmclassification.h"

using namespace RMCCORE;
using namespace std;

#define DEFAULT_PROJECTS_PROFILE		"DProjectData.txt"
#define DEFAULT_PROJECTS_FILES		    "DProjectFiles.txt"

void TestClassification();

const RMMyProjects & GetProjects(void)
{
	JsonDocument doc;
	int err_code = 0;
	size_t err_pos = 0;

	const std::string jsonstr = ReadFromFile(DEFAULT_PROJECTS_PROFILE);
	if (!doc.LoadJsonString(jsonstr, &err_code, &err_pos)) {
		throw;
	}

	JsonValue *root = doc.GetRoot();
	if (!(NULL != root && root->IsObject())) {
		throw;
	}

	static RMMyProjects project;
	project.ImportFromJson(root->AsObject());

	return project;
}

void TestProject()
{
	TestClassification();

	RMUser user = GetDefaultUser();

	RMMyProjects *myproject = user.GetMyProjects();
	cout << "Query Project List:" << endl;

	const std::string jsonstr = ReadFromFile("RMListProject.txt");

	RetValue retv = myproject->ImportFromRMSResponse(jsonstr);

	WriteToFile(DEFAULT_PROJECTS_PROFILE, myproject->ExportToString());

	const std::string pathid = "/Project1/";
	
	unsigned int projid = 13;
	HTTPRequest request;
	
	request = user.GetProjectUploadFileQuery(projid, pathid, "RMSToken.txt.nxl", false);
	printHttpRequest(request);
	cout << endl;

	RMProject * project = (RMProject *)myproject->GetProject(0);
	RMProjectFiles *projectfiles = (RMProjectFiles *)project->GetProjectFiles();

	cout << "Query Project Files:" << endl;

	const std::string jsonstr1 = ReadFromFile("RMProjectFiles.txt");

	retv = projectfiles->ImportFromRMSResponse(jsonstr1);

	std::vector<RMProjectFile>* pfilevec = projectfiles->GetProjectFile();
	std::string pathId;
	for (RMProjectFile pfile : *pfilevec)
	{
		cout << "fileName:" << pfile.GetFileName() << endl;
		cout << "pathId:" << pfile.GetPathId() << endl;
	}
	cout << " " <<  endl;
	WriteToFile(DEFAULT_PROJECTS_FILES, projectfiles->ExportToString());

}

void TestClassification()
{
	RMUser user = GetDefaultUser();

	unsigned int projid = 13;
	HTTPRequest request;

	RMClassification * myClassification = user.GetMyClassification();
	HTTPRequest classificationquery = myClassification->GetClassificationQuery(projid);
	printHttpRequest(classificationquery);
	cout << endl;

	RMClassification classification;
	const std::string jsonstr = ReadFromFile("RMClassification.txt");

	RetValue retv = classification.ImportFromRMSResponse(jsonstr);
	std::vector<CLASSIFICATION_CAT> cat = classification.GetClassificationCategories();
}