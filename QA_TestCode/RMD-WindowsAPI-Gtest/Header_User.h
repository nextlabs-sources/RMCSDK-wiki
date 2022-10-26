#pragma once
#include "stdafx.h"
#include "SDLInc.h"


bool Default_Login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password);
bool PreviousUser_Login(ISDRmcInstance *&pInstance, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, string passcode);
//bool Switch_User(ISDRmcInstance *&pInstance, ISDRmUser *&puser, const string email, const string password);

bool SDKRPM_Login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password);
bool Admin_Login(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&puser, const wstring &router, const wstring &tenant, const string email, const string password, string id);