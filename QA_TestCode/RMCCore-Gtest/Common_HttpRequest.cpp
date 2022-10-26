//
//  Created by Lifi.Yuan on 3/14/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#include "stdafx.h"
//#include "rmsdk\network\httpClient.h"

void printHttpRequest(const HTTPRequest request)
{
	if (request.GetURL().length() == 0) {
		cout << "Invalid HTTP request" << endl;
		return;
	}
	cout << "HTTP URL: " << request.GetURL().c_str() << endl;
	cout << "HTTP Method:" << request.GetMethod().c_str() << endl;
	cout << "HTTP Header:";
	if (request.GetHeaders().size() == 0) {
		cout << "None" << endl;
	}
	else {
		cout << endl;
		std::for_each(
			request.GetHeaders().begin(), request.GetHeaders().end(), [&](const http::header item) {
			//assert(item.second.c_str());
			cout << item.first.c_str() << ":" << item.second.c_str() << endl;
		});
	}
	cout << "HTTP cookies:";
	if (request.GetCookies().size() == 0) {
		cout << "None" << endl;
	}
	else {
		cout << endl;
		std::for_each(request.GetCookies().begin(), request.GetCookies().end(), [&](const http::cookie item) {
			cout << item.first.c_str() << ":" << item.second.c_str() << endl;
		});
	}
	cout << "HTTP AcceptTypes:";
	if (request.GetAcceptTypes().size() == 0) {
		cout << "None" << endl;
	}
	else {
		cout << endl;
		for_each(request.GetAcceptTypes().begin(), request.GetAcceptTypes().end(), [&](const std::string item) {
			cout << item.c_str() << ";";
		});
		cout << endl;
	}
	if (request.GetBodyLength()) {
		cout << "HTTP Body:" << endl;
		cout << request.GetBody().c_str() << endl;
	}
}

/*HTTPRequestType value means:
1: GetUserLoginURL
2: GetMembershipQuery
3: GetMembershipTokenQuery
4: GetShareLocalFileQuery
*/
exceptedHTTPRequest setValue(int HTTPRequestType, string header, string cookies, string body, RMTenant tenant)
{
	exceptedHTTPRequest e;
	e.Headers = header;
	e.Cookies = cookies;
	e.Body = body;

	switch (HTTPRequestType)
	{
	case 1:
	{
		//GetUserLoginURL
		//e.URL = "https://skydrm.com/rms/Login.jsp?tenant=skydrm.com";
		e.URL = tenant.GetRMSURL() + "/Login.jsp?tenant=" + tenant.GetTenant();
		e.Method = "GET";
		e.AcceptType1 = "application/json";
		e.AcceptType2 = "text";
		break;
	}
	case 2:
	{
		//GetMembershipQuery
		//e.URL = "https://skydrm.com/rms/rs/membership";
		e.URL = tenant.GetRMSURL() + "/rs/membership";
		e.Method = "PUT";
		e.AcceptType1 = "application/json";
		e.AcceptType2 = "text";
		break;
	}
	case 3:
	{
		//GetMembershipTokenQuery
		//e.URL = "https://skydrm.com/rms/rs/token";
		e.URL = tenant.GetRMSURL() + "/rs/token";
		e.Method = "PUT";
		e.AcceptType1 = "application/json";
		e.AcceptType2 = "text";
		break;
	}
	case 4:
	{
		e.URL = tenant.GetRMSURL() + "/rs/share/local";
		e.Method = "POST";
		e.AcceptType1 = "application/json";
		e.AcceptType2 = "";
		break;
	}
	default:
	{
		//GetUserLoginURL
		e.URL = tenant.GetRMSURL() + "/Login.jsp?tenant=" + tenant.GetTenant();
		e.Method = "GET";
		e.AcceptType1 = "application/json";
		e.AcceptType2 = "text";
		break;
	}
	}

	return e;
}

void check_httpRequest(exceptedHTTPRequest e, HTTPRequest request, bool BodyFuzzy)
{
	string headers = "";
	string cookies = "";

	EXPECT_STREQ(e.URL.c_str(), request.GetURL().c_str());
	EXPECT_STREQ(e.Method.c_str(), request.GetMethod().c_str());

	std::for_each(request.GetHeaders().begin(), request.GetHeaders().end(), [&](const http::header item) {
		//cout << item.first.c_str() << ":" << item.second.c_str() << endl;
		headers.append(item.first.c_str());
		headers.append(item.second.c_str());
	});
	EXPECT_STREQ(e.Headers.c_str(), headers.c_str());

	std::for_each(request.GetCookies().begin(), request.GetCookies().end(), [&](const http::cookie item) {
		//cout << item.first.c_str() << ":" << item.second.c_str() << endl;
		cookies.append(item.first.c_str());
		cookies.append(item.second.c_str());
	});
	EXPECT_STREQ(e.Cookies.c_str(), cookies.c_str());

	//if (e.AcceptType1.c_str() == request.GetAcceptTypes.c_str())
	//EXPECT_STREQ(e.AcceptType1.c_str(), request.GetAcceptTypes.c_str());
	//else
	//	EXPECT_STREQ(e.AcceptType2.c_str(), request.GetAcceptTypes.c_str());

	if (BodyFuzzy)
	{
		string exceptBody = e.Body;
		string gotBody = request.GetBody();
		string gotBodyFront = gotBody.substr(0, exceptBody.size());
		string tail = "\"}}";
		string gotBodyTail = gotBody.substr(gotBody.size() - tail.size(), tail.size());
		int e_size = exceptBody.size() + 1080 + tail.size();
		//cout << gotBodyFront.compare(exceptBody) << endl;
		//cout << gotBody.compare(gotBody.size() - tail.size(), tail.size(), tail)<< endl;

		//cout << gotBody.size() << endl;

		EXPECT_STREQ(exceptBody.c_str(), gotBodyFront.c_str());
		EXPECT_STREQ(tail.c_str(), gotBodyTail.c_str());
		EXPECT_EQ(e_size, gotBody.size());

		//if ((gotBodyFront.compare(exceptBody) == 0) && (gotBody.compare(gotBody.size() - tail.size(), tail.size(), tail) == 0))
		//{
		//	cout << "HTTP Body include the expected value" << endl;
		//}
		//else
		//{
		//	cout << "Http Body fuzzy matching failed:" << endl;
		//	EXPECT_STREQ(e.Body.c_str(), request.GetBody().c_str());
		//}
	}
	else
	{
		cout << "Http Body fuzzy matching failed:" << endl;
		EXPECT_STREQ(e.Body.c_str(), request.GetBody().c_str());
	}

}

string getJsonPara_tags(string tag_csv)
{
	string Para_tags;
	string b_comma = ",";
	string t_tolower = tag_csv;
	transform(t_tolower.begin(), t_tolower.end(), t_tolower.begin(), ::tolower);
	if (t_tolower == "null")
		Para_tags = "";
	else
	{
		//tag details
		Para_tags = "\"tags\":{";

		//split each tags with "|"
		vector<string> tags;
		splitString(tag_csv, tags, "|");
		for (vector<string>::size_type i = 0; i != tags.size(); i++)
		{
			//Split tag name and value with "="
			vector<string> t;
			splitString(tags[i], t, "=");
			Para_tags = Para_tags + "\"" + t[0] + "\":[";

			//Split each value with ";"
			vector<string> t_values;
			splitString(t[1], t_values, ";");
			for (vector<string>::size_type j = 0; j != t_values.size(); j++)
			{
				Para_tags = Para_tags + "\"" + t_values[j] + "\"";
				if (j < t_values.size() - 1)
				{
					Para_tags = Para_tags + b_comma;
				}
			}

			Para_tags = Para_tags + "]";
			if (i < tags.size() - 1)
			{
				Para_tags = Para_tags + b_comma;
			}
		}

		Para_tags = Para_tags + "}";
		Para_tags = Para_tags + b_comma;
	}

	return Para_tags;
}

string getJsonPara_expiry(string expiry_option, const char* expiry_startDate, const char* expiry_endDate)
{
	string b_comma = ",";
	expiry_option = "\"option\":" + expiry_option;

	string b_startDate = "";
	string b_endDate = "";
	string s_date = expiry_startDate;
	if (s_date != "null")
	{
		int startDate = standard_to_stamp(expiry_startDate);
		b_startDate = b_comma + "\"startDate\":" + to_string(startDate);
	}
	string e_date = expiry_endDate;
	if (e_date != "null")
	{
		int endDate = standard_to_stamp(expiry_endDate);
		b_endDate = b_comma + "\"endDate\":" + to_string(endDate);
	}

	string Para_expiry = b_comma + "\"expiry\":{" + expiry_option + b_startDate + b_endDate + "}";

	return Para_expiry;
}

string getJsonPara_oneValue(string ParaName, string ParaValue)
{
	string b_comma = ",";
	string Para_tolower = ParaValue;
	transform(Para_tolower.begin(), Para_tolower.end(), Para_tolower.begin(), ::tolower);
	if (Para_tolower == "null")
		ParaValue = "";
	if (ParaValue != "")
	{
		ParaValue = "\"" + ParaName + "\":\"" + ParaValue + "\"";
		ParaValue = b_comma + ParaValue;
	}

	return ParaValue;
}

string getJsonPara_recipients(vector<std::string> recipients)
{
	string Para;
	string b_comma = ",";

	if (recipients.size() == 0)
		Para = "\"recipients\":[]";
	else
	{
		for (vector<string>::size_type i = 0; i != recipients.size(); i++)
		{
			if (i == 0)
				Para = "\"recipients\":[";
			else
				Para = Para + b_comma;

			Para = Para + "{\"email\":\"" + recipients[i] + "\"}";
		}
		Para = Para + "]";
	}

	return Para;
}

bool Check_HttpHeader(HTTPRequest request, string content_type, RMUser user) {
	string header_str;
	string expect_header;
	ExportToFile("user.txt", user.ExportToString());
	Json::Value jroot;
	jroot = ParseJsonFile("ex_user.txt");
	string tenant = jroot["TenantInfo"]["Tenant"].asString();
	string client_id = jroot["SystemInfo"]["ClientGUID"].asString();
	string platform_id = jroot["SystemInfo"]["PlatformID"].asString();
	string ticket = jroot["Ticket"].asString();
	string user_id = jroot["Id"].asString();
	string device = jroot["SystemInfo"]["DeviceName"].asString();
	if (content_type != "") {
		content_type = "Content-Type" + content_type;
	}
	expect_header = "clientId" + client_id + "platformId" + platform_id + "deviceId" + device + "tenant" + tenant + "userId" + user_id + "ticket" + ticket + content_type;
	if (request.GetHeaders().size() == 0) {
		cout << "None" << endl;
		return false;
	}
	else {
		std::for_each(request.GetHeaders().begin(), request.GetHeaders().end(), [&](const http::header item) {
			header_str.append(item.first.c_str());
			header_str.append(item.second.c_str());
		});
		if (header_str.c_str() == expect_header.c_str())
			return true;
	}
}

bool Check_HttpURL_Method(HTTPRequest request, string method, string url, RMUser user) {
	string expect_url;
	ExportToFile("user.txt", user.ExportToString());
	Json::Value jroot;
	jroot = ParseJsonFile("ex_user.txt");
	string server = jroot["TenantInfo"]["ServerURL"].asString();
	expect_url = server + url;
	string server_url = request.GetURL();
	if ((request.GetMethod() == method) && (request.GetURL() == expect_url))
	{
		return true;
	}
	return false;

}

bool Check_Http_Body(HTTPRequest request, string activity_log) {
	string body = request.GetBody();
	body = body.substr(0, body.length() - 1);
	if (body == activity_log) {
		return true;
	}
	else {
		return false;
	}
}
