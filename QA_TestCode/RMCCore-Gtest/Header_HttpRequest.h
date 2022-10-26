#pragma once

#include "rmccore/network/httpClient.h"

void printHttpRequest(const RMCCORE::HTTPRequest request);

struct exceptedHTTPRequest
{
	string URL;
	string Method;
	string Headers;
	string Cookies;
	string AcceptType1;
	string AcceptType2;
	string Body;
};
exceptedHTTPRequest setValue(int HTTPRequestType, string header, string cookies, string body, RMTenant tenant);
void check_httpRequest(exceptedHTTPRequest e, HTTPRequest request, bool BodyFuzzy = false);

void checkErrorCode(RetValue ret, string jsonFile);

string getJsonPara_tags(string tag_csv);
string getJsonPara_expiry(string expiry_option, const char* expiry_startDate, const char* expiry_endDate);
string getJsonPara_oneValue(string ParaName, string ParaValue);
string getJsonPara_recipients(vector<std::string> recipients);

bool Check_HttpHeader(HTTPRequest request, string content_type, RMUser user);
bool Check_HttpURL_Method(HTTPRequest request, string method, string url, RMUser user);
bool Check_Http_Body(HTTPRequest request, string activity_log);