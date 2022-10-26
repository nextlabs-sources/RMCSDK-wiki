//
//  common.cpp
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 08/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#include "common.hpp"
#include "assert.h"
#include "rmsdk/rmsdk.h"
#include "rmsdk/utils/string.h"

using namespace RMSDK;
using namespace std;

#define Tenant_Return "{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1516331538565,\"results\":{\"server\":\"https://skydrm.com/rms\"}}"
//#define Tenant_Return  "{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505538781,\"results\":{\"server\":\"https://skydrm.com/rms\"}}"

const std::string readFromFile(std::string filename)
{
    std::string retstr;
    
    FILE * testfile;
    testfile = fopen(filename.c_str(), "r");
//    fopen_s(&testfile, filename.c_str(), "r");
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

const std::string generateClientID(void)
{
    std::string clientid;
    for (int i = 0; i < 16; i++) {
        unsigned char r = (unsigned char)std::rand();
        std::string tmp = utohs<char>(r);
        clientid += tmp;
    }

    return clientid;
    
    return "";
}

const RMTenant createDefaultTenant(void)
{
    RMTenant tenant;
    
    JsonDocument doc;
    int err_code = 0;
    size_t err_pos = 0;
    
    assert(doc.LoadJsonString(Tenant_Return, &err_code, &err_pos));
    
    JsonValue* root = doc.GetRoot();
    assert(NULL != root);
    assert(root->IsObject());
    
    tenant.ImportFromRMSResponse(root->AsObject());
    
    return tenant;
}

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

void printIDPType(int type)
{
    //     typedef enum _USERIDPTYPE {
    //         SKYDRM = 0,
    //         SAML,
    //         GOOGLE,
    //         FACEBOOK,
    //         YAHOO
    //     } USER_IDPTYPE;
    
    switch (type)
    {
        case 0:
            cout << " SkyDRM";
            break;
        case 1:
            cout << " SAML";
            break;
        case 2:
            cout << " Google";
            break;
        case 3:
            cout << " FaceBook";
            break;
        case 4:
            cout << " Yahoo";
            break;
        default:
            cout << " Unknown";
            break;
    }
}

void printMembership(RMMembership & mem)
{
    cout << "Member ID:" << mem.GetID() << endl;
    cout << "Member tenant ID: " << mem.GetTenantID() << endl;
    cout << "Member Project ID:" << mem.GetProjectID() << endl;
    cout << "Member IDP Type: " << mem.GetIdpType() << endl;
}
