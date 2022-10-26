#include "stdafx.h"
#include "HttpReqest.h"

#include <algorithm>

using namespace std;
using namespace RMCCORE;

void printHttpRequest(const HTTPRequest request)
{
	if (request.GetURL().length() == 0 ) {
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
		std::for_each(request.GetHeaders().begin(), request.GetHeaders().end(), [&](const http::header item) {
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