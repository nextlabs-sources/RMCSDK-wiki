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
#include "rmccore/restful/rmremoteviewer.h"

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
			RMActivityLog& activitylog, RMNXLFile& rmnxfile, const std::string& memberid = "", bool bNoChange = false);
#ifdef _WIN32
		RetValue ProtectLocalFile(const std::wstring& filepath, const std::wstring& destfilepath, const NXLAttributes & attrs, RMToken& token, 
			 RMActivityLog& activitylog, RMNXLFile& rmnxfile, const std::string& memberid="");
		RetValue ShareLocalFile(const std::wstring& filepath, const std::wstring& destfilepath, const NXLAttributes & attrs, const std::vector<std::string>& recipients, RMToken& token, const std::wstring & comments,
			               RMActivityLog& activitylog, RMNXLFile& sharefile, const std::string& memberid = "");
		RetValue ReProtectLocalFile(const std::wstring& filepath, const std::wstring& destfilepath, const NXLAttributes & attrs, RMToken& token,
			RMActivityLog& activitylog, RMNXLFile& rmnxfile, const std::string& memberid = "", bool bNoChange = false, bool bNoChangeIssueTime = false);
#endif
	public:
		HTTPRequest GetUserLoginURL(void);
		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);

	public: //Login API
        HTTPRequest GetUserLogoutQuery(void);
		RetValue Logout(JsonObject * rms_response_root);

	public: //User API
		HTTPRequest GetBasicProfileQuery(void);
		HTTPRequest GetProfileQuery(void);
		HTTPRequest GetUpdateUserPreferenceQuery(const uint32_t option, const uint64_t start = 0, const uint64_t end = 0, const std::wstring watermark = L"");
		HTTPRequest GetUserPreferenceQuery();
		HTTPRequest GetTenantPreferenceQuery(const std::string& tenantid="");

	public: //membership API
		HTTPRequest GetMembershipQuery(const RMMembership& membership);
    public: //Token API
        HTTPRequest GetMembershipTokenQuery(const RMMembership& membership, int tokencount);
		HTTPRequest GetMembershipTokenQuery(const RMMembership& membership, int protectionType, const std::string &fileTagsOrPolicy);
		HTTPRequest GetFileTokenQuery(const RMNXLFile & file, const uint32_t protectionType = 0, const std::string& filePolicy = "", const std::string& fileTags = "", const std::string& dynamicEvalRequest = "");
		HTTPRequest GetFileTokenQuery(const std::string& file_ownerid, const std::string &agreement, const std::string& duid, uint32_t ml,
			const uint32_t protectionType = 0, const std::string& filePolicy = "", const std::string& fileTags = "", const std::string& dynamicEvalRequest = "");

	public: //Share API
        HTTPRequest GetShareLocalFileQuery(const RMMembership & membership, const RMNXLFile & file);
        HTTPRequest GetUpdateRecipientsQuery(const RMNXLFile & file, const RMRecipients &recipients);
		HTTPRequest GetShareFromMyVaultQuery(const RMNXLFile & file, const std::vector<std::string> &recipients, const std::string &repositoryId, const std::string &fileName, const std::string &filePathId, const std::string &filePath, const std::string &comment, const std::string &membershipid, uint64_t permission);
	public: //MyVault API
        HTTPRequest GetProtectLocalFileQuery(const RMNXLFile & file);
        HTTPRequest GetNXLFileMetadataQuery(const RMNXLFile & file);
		HTTPRequest GetNXLFileMetadataQuery(const std::string& duid, const std::string& pathid);
	public: //Project API
		HTTPRequest GetProtectProjectFileQuery(const std::string &projectid, const std::string &destfolder, const RMNXLFile & file, bool overwrite = false);
		HTTPRequest GetProjectFileMetadataQuery(const std::string& projectid, const std::string& pathid);
	public://activity log APIs
		HTTPRequest GetUploadActivitylogQuery(RMLogPool* logpool, int lognum = 0);
        HTTPRequest GetNXLFileActivitylogQuery(const RMNXLFile & file, uint64_t startPos, uint64_t count, const RMLogSearchField searchField, const std::string &searchText, const RMLogOrderBy orderByField, bool orderByReverse);
	public://Heartbeat API
		HTTPRequest GetHeartBeatQuery(const RMHeartbeat & heartbeat);
		HTTPRequest GetProjectUploadFileQuery(unsigned int projid, const std::string &pathid, const std::string &filename, bool nxlfile = true);
      
		HTTPRequest UpdateNXLMetadataQuery(const std::string& duid, const std::string& otp, const std::string& filePolicy, const std::string& fileTags, unsigned int ml);
		HTTPRequest UpdateNXLMetadataExQuery(const std::string& duid, const std::string& fileTags, const std::string& existingFileTags, const std::string& fileHeader, unsigned int ml);
		HTTPRequest ClassifyProjectFileQuery(unsigned int projid, const std::string& filename, const std::string& parentPathId, const std::string& fileTags);

	public: //Remote Viewer API
		HTTPRequest GetUploadAndViewLocalFileQuery(const RMNXLFile& file, uint64_t operations);
		HTTPRequest GetViewRepoFileQuery(const std::string& repositoryid, const RMCCORE::RMNXLFile &file, const std::string& repositoryname, const std::string& repositorytype, uint64_t operations);
		HTTPRequest GetViewProjectFileQuery(unsigned int projid, const RMNXLFile& file, uint64_t operations);

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
        
		HTTPRequest    m_uploadlogRequest;

        RMMembership   m_defaultmembership;
        std::vector<RMMembership> m_memberships;
		std::vector<std::string> m_emails;
		std::vector<std::pair<std::wstring, std::wstring>> m_attributes;
		std::vector<std::pair<std::wstring, std::vector<std::wstring>>> m_attrs;
    };
}

#endif /* __RMCCORE_RESTFUL_RMUSER_H__ */
