#pragma once
#include "SDLInc.h"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
void GetUserLoginResponseData(const string email, const string password, const string clientID, const string userReturnFilePath);
SDWLResult CreateSDRmcInstance_CreateClientID(const WCHAR *tempfolder, ISDRmcInstance *&pInstance, string &clientId, const wstring router_ws, const wstring tenant_ws, const WCHAR *sdklibfolder = L"C:\\Program Files\\NextLabs");

void GetUserLoginResponseData_InputServer(const string email, const string password, const string clientID, const string userReturnFilePath, const string RMSname);

string getUserIDFromJson(const string JosnFilePath);

SDWLResult userFirstTimeLogin(const string email, const string password, const string testFolder, wstring &tenantName, string &passcode);