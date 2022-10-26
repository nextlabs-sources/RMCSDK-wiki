#pragma once
#include "SDLInc.h"


void checkSDWLResult(SDWLResult res);
void checkResult(SDWLResult res, int expected);

const std::vector<SDRmFileRight> Tick_Rights(string rights_str);
const std::vector<std::string> Get_Recipients(string recipients_str);

bool checkFileExist(string filefolder, string filename);
void deleteFile(string filefolder, string filename);

void getLastCreatedNXL(string path, string &lastCreatedNXLFile);

void createFolder(const string &path);

std::string GetLastErrorAsString();

void printPolicyBaseOnTenant(wstring & tenantName, ISDRmUser *&puser);
void printPolicy(ISDRmcInstance *pInstance, ISDRmUser *&puser);
void waitInstanceInitFinish(ISDRmcInstance *&pInstance);

string readFileContent(string fileDir, string fileName);
string readFileBeginningChar(string file, int charNum);

//code in RPM.cpp
void CopyviaCMD(string source, string dest);
void prepareTestNXLFiles(ISDRmUser* puser, string sourceFile, std::vector<SDRmFileRight> rights, std::vector<std::string> recipients, wstring comments, SDR_WATERMARK_INFO watermarkinfo, SDR_Expiration expire, string tags, string membershipId, wstring &NXL_myValut, wstring &NXL_project);
//code in JPC.cpp
void printRightsOnly(const SDWLResult res, const vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks);
void printEvaluationResultRights(const SDWLResult res, const vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks);

string getTime();
void getAllFiles(string path, vector<string>& files);

bool file_exist(string file_path);
void edit_file(string file_path, string content);
string read_filecontent(string file_path);
BOOL FindProcessPid(LPCSTR ProcessName, DWORD& dwPid);
string GetFileShortName(wstring longFilePath);
void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
vector<SDRmFileRight> getRightsString(int rights_value);
bool is_projectowner(int pro_id, string email);
bool is_projectmember(int pro_id, string email);
bool checkRights(int rights, string rights_check);
int rights_changeToValue(vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks);
int rights_changeToValue2(vector<SDRmFileRight> filerights);
