//
//  rmuser.h
//  rmccore
//
//  Created by Haobo Wang on 11/30/17.
//  Copyright © 2017 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMUSER_H__
#define __RMCCORE_RESTFUL_RMUSER_H__

#include "rmrestbase.h"
#include "rmmembership.h"
#include "rmtenant.h"
#include "rmmydrive.h"
#include "rmproject.h"
#include "rmprojectfile.h"
#include "rmccore/utils/json.h"
#include "rmccore/network/httpClient.h"
#include "rmccore/format/nxlpolicy.h"
#include "rmccore/restful/rmnxlfile.h"
#include "rmccore/restful/rmsyspara.h"
#include "rmccore/restful/rmactivitylog.h"
#include "rmccore/restful/rmrecipients.h"
#include "rmccore/restful/rmheartbeat.h"
#include "rmccore/restful/rmclassification.h"

namespace RMCCORE {
	class RMUser : public RestfulBase
	{
	public:
		RMUser(void);
		RMUser(const RMSystemPara &sys, const RMTenant & tenant) throw(...);
        RMUser(JsonObject * value) throw(...);
		~RMUser();
	public:
		const std::string GetName() { return m_name; }
		const std::string GetEmail() { return m_email; }
		const std::vector<std::string> GetEmails() { return m_emails; }
		unsigned int GetIdpType() { return m_idptype; }
		unsigned int GetUserID() { return m_userid; }
		RMMembership &GetDefaultMembership() { return m_defaultmembership; }
		RMMembership* FindMembership(std::string tenantid);
		RMMembership* FindMembershipFromID(std::string id);
		RMUser& operator = (const RMUser& rhs);

		RetValue UpdateSystemParameters(const RMSystemPara & param);
        RetValue UpdateTenant(const RMTenant & tenant);
		const RMTenant & GetTenant(void) { return m_tenant; }
		RMMyDrive * GetMyDrive();
		RMMyVault * GetMyVault();
		RMSharedWithMe * GetSharedWithMe();
		RMMyProjects * GetMyProjects();
		RMClassification* GetMyClassification() { return &m_classification; }
		
	public:
		bool IsLogin() { return m_login; }
		bool IsLoginExpired() { return (0 == GetExpiredTime()); }
		uint64_t	GetExpiredTime();

	public:
		RetValue ProtectLocalFile(const std::string& filepath, const std::string& destfilepath, const NXLAttributes & attrs, RMToken& token, 
			               RMActivityLog& activitylog, RMNXLFile& rmnxlfile, const std::string& memberid = "");
		RetValue ShareLocalFile(const std::string& filepath, const std::string& destfilepath, const NXLAttributes & attrs, const std::vector<std::string>& recipients, RMToken& token, const std::string & comments,
			               RMActivityLog& activitylog, RMNXLFile& sharefile, const std::string& memberid = "");
		RetValue ReProtectLocalFile(const std::string& filepath, const std::string& destfilepath, const NXLAttributes & attrs, RMToken& token,
			RMActivityLog& activitylog, RMNXLFile& rmnxfile, const std::string& memberid = "");
#ifdef _WIN32
		RetValue ProtectLocalFile(const std::wstring& filepath, const std::wstring& destfilepath, const NXLAttributes & attrs, RMToken& token, 
			 RMActivityLog& activitylog, RMNXLFile& rmnxfile, const std::string& memberid="");
		RetValue ShareLocalFile(const std::wstring& filepath, const std::wstring& destfilepath, const NXLAttributes & attrs, const std::vector<std::string>& recipients, RMToken& token, const std::wstring & comments,
			               RMActivityLog& activitylog, RMNXLFile& sharefile, const std::string& memberid = "");
		RetValue ReProtectLocalFile(const std::wstring& filepath, const std::wstring& destfilepath, const NXLAttributes & attrs, RMToken& token,
			RMActivityLog& activitylog, RMNXLFile& rmnxfile, const std::string& memberid = "");
#endif
	public:
		const HTTPRequest &GetUserLoginURL(void);
		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);

	public: //Login API
        const HTTPRequest &GetUserLogoutQuery(void);
		RetValue Logout(JsonObject * rms_response_root);

	public: //User API
		const HTTPRequest &GetBasicProfileQuery(void);
		const HTTPRequest &GetProfileQuery(void);
		const HTTPRequest &GetUpdateUserPreferenceQuery(const uint32_t option, const uint64_t start = 0, const uint64_t end = 0, const std::wstring watermark = L"");
		const HTTPRequest &GetUserPreferenceQuery();
		const HTTPRequest &GetTenantPreferenceQuery(const std::string& tenantid="");

	public: //membership API
		const HTTPRequest &GetMembershipQuery(const RMMembership& membership);
    public: //Token API
        const HTTPRequest &GetMembershipTokenQuery(const RMMembership& membership, int tokencount);
		const HTTPRequest &GetFileTokenQuery(const RMNXLFile & file);
		const HTTPRequest &GetFileTokenQuery(const std::string& file_ownerid, const std::string &agreement, const std::string& duid, uint32_t ml);

	public: //Share API
        const HTTPRequest &GetShareLocalFileQuery(const RMMembership & membership, const RMNXLFile & file);
        const HTTPRequest &GetUpdateRecipientsQuery(const RMNXLFile & file, const RMRecipients &recipients);
    public: //MyVault API
        const HTTPRequest &GetProtectLocalFileQuery(const RMNXLFile & file);
        const HTTPRequest &GetNXLFileMetadataQuery(const RMNXLFile & file);
		const HTTPRequest &GetNXLFileMetadataQuery(const std::string& duid, const std::string& pathid);
	public: //Project API
		const HTTPRequest &GetProtectProjectFileQuery(const std::string &projectid, const std::string &destfolder, const RMNXLFile & file, bool overwrite = false);
		const HTTPRequest &GetProjectFileMetadataQuery(const std::string& projectid, const std::string& pathid);
	public://activity log APIs
		const HTTPRequest &GetUploadActivitylogQuery(RMLogPool* logpool, int lognum = 0);
        const HTTPRequest &GetNXLFileActivitylogQuery(const RMNXLFile & file, uint64_t startPos, uint64_t count, const RMLogSearchField searchField, const std::string &searchText, const RMLogOrderBy orderByField, bool orderByReverse);
	public://Heartbeat API
		const HTTPRequest &GetHeartBeatQuery(const RMHeartbeat & heartbeat);
		const HTTPRequest &GetProjectUploadFileQuery(unsigned int projid, const std::string &pathid, const std::string &filename, bool nxlfile = true);
        
    public:
        RetValue ImportFromString(std::string jsonstr);
        RetValue ImportFromJson(JsonObject * value);
        
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
	protected:
		std::string BuildFileMetadata(const std::string filename, const std::string fileext, const std::string createdby = "", uint64_t datecreated = 0, const std::string modifiedby = "", uint64_t datemodified = 0);
    protected:
        RMTenant       m_tenant;
        RMSystemPara   m_params;
        
        RMMyDrive      m_mydrive;
		RMMyVault      m_myvault;
		RMSharedWithMe m_sharedwithme;
		RMMyProjects   m_myprojects;
		RMProjectFiles m_myprojectfiles;
        bool		   m_login;
		uint64_t	   m_ttl;
		uint64_t	   m_login_servertime;
		uint64_t	   m_login_clienttime;
		
		std::string    m_name;
        std::string    m_email;
        unsigned int   m_userid;
        std::string    m_ticket;
        std::string    m_default_tenantid;
        unsigned int   m_idptype;
		RMClassification m_classification;
        
		HTTPRequest	   m_loginRequest;
        HTTPRequest    m_MembershipQuery;
        HTTPRequest    m_TokenQuery;
		HTTPRequest    m_FileTokenReqeust;
		HTTPRequest	   m_ShareLocalQuery;
		HTTPRequest	   m_logoutRequest;
		HTTPRequest    m_uploadlogRequest;
        HTTPRequest    m_updateRecipientsReqest;
        HTTPRequest    m_getFileMetadataRequest;
		HTTPRequest    m_getProjectFileMetadataRequest;
        HTTPRequest    m_ProtectLocalQuery;
		HTTPRequest    m_ProtectProjectQuery;
		HTTPRequest    m_getFileLogRequest;
		HTTPRequest	   m_HeartbeatQuery;
		HTTPRequest	   m_ProfileQuery;
		HTTPRequest	   m_BasicProfileQuery;
		HTTPRequest	   m_ProjectUploadFileQuery;
		HTTPRequest	   m_UpdateUserPreferenceQuery;
		HTTPRequest	   m_GetUserPreferenceQuery;
		HTTPRequest	   m_GetTenantPreferenceQuery;

        RMMembership   m_defaultmembership;
        std::vector<RMMembership> m_memberships;
		std::vector<std::string> m_emails;
		std::vector<std::pair<std::wstring, std::wstring>> m_attributes;
		std::vector<std::pair<std::wstring, std::vector<std::wstring>>> m_attrs;
    };
}

#endif /* __RMCCORE_RESTFUL_RMUSER_H__ */
