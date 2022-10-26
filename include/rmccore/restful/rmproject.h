/*!
 * \class rmproject
 *
 * \brief main class for project
 *
 * \author hbwang
 * \date May 2018
 */
#pragma once
#ifndef __RMCCORE_RESTFUL_RMPROJECT_H__
#define __RMCCORE_RESTFUL_RMPROJECT_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/network/httpClient.h"
#include "rmccore/restful/rmsyspara.h"
#include "rmccore/restful/rmtenant.h"
#include "rmccore/restful/rmprojectfile.h"

namespace RMCCORE {
	class RMProjectMember : public RestfulBase
	{
	public:
		RMProjectMember();
		~RMProjectMember();

	public:
		unsigned int GetUserID(void) const { return m_userid; }
		std::string GetUserName(void) const { return m_username; }
		std::string GetUserEmail(void) const { return m_useremail; }
		uint64_t	GetJoinTime(void) const { return m_jointime; }
	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
	protected:
		unsigned int m_userid;
		std::string m_username;
		std::string m_useremail;
		uint64_t	m_jointime;

	};

	typedef enum {
		PATProjectTrial = 0,
		PATProject,
		PATEnterprise
	} ProjectAccountType;
	typedef enum  {
		PFAll = 0,
		PFOwnedByMe,
		PFOwnedByOther
	} ProjectFilter;
	typedef enum {
		// actual values must match RMS RESTful API spec
		PFDTNormal       = 0,
		PFDTForViewer    = 1,
		PFDTForOffline   = 2
	} ProjectFileDownloadType;

	class RMProject : public RestfulBase
	{
	public:
		RMProject();
		~RMProject();
	public:
		bool IsInitialized(void) { return m_initialized; }
		unsigned int GetProjectID(void) const { return m_projid; }
		const std::string & GetProjectName(void) const { return m_name; }
		const std::string & GetDisplayName(void) const { return m_displayname; }
		const std::string & GetDescription(void) const { return m_description; }
		const std::string & GetTenantId(void) const { return m_tenantId; }

		bool IsOwnbyMe(void) const { return m_bowner; }
		ProjectAccountType GetAccountType(void) const { return m_accounttype; }
		
		unsigned int GetOwnerID(void) const { return m_ownerid; }
		const std::string & GetOwnerName(void) const { return m_ownername; }
		const std::string & GetOwnerEmail(void) const { return m_owneremail; }
		const uint64_t GetTotalFiles(void) const { return m_totalfiles; }

		const RMProjectFiles* GetProjectFiles() { return &m_projectFiles; };

		RMProject& operator = (const RMProject& rhs);


	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);

		// project files list
		const HTTPRequest &GetListFilesQuery(unsigned int projectId, uint32_t pageId, uint32_t pageSize, const std::string& orderBy, const std::string& pathId, const std::string& searchString);
	
		RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);

	private:
		bool          m_initialized;
		RMSystemPara  m_params;
		RMTenant      m_tenant;
		uint32_t      m_userid;
		std::string   m_ticket;
		 
		uint32_t      m_projid;
		std::string   m_name;
		std::string   m_displayname;
		std::string   m_description;
		std::string   m_tenantId;
		bool          m_bowner;
		uint64_t	  m_totalfiles;
		uint64_t	  m_createtime;
		uint64_t	  m_expiretime;
		ProjectAccountType  m_accounttype;
		uint32_t      m_ownerid;
		std::string    m_ownername;
		std::string    m_owneremail;
		HTTPRequest	   m_listProjectFilesQuery;
		RMProjectFiles m_projectFiles;

		std::vector<RMProjectMember> m_memberslist;
		std::string m_invitationmsg;
	};

	class RMMyProjects : public RestfulBase
	{
	public:
		RMMyProjects();
		~RMMyProjects();

		bool IsInitialized(void) { return m_initialized; }
	public:
		size_t	GetProjectNumber(void) { return m_projectlist.size(); }
		const RMProject* GetProject(uint32_t numbr);
		

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
	public://Project API
		const HTTPRequest &GetListProjectsQuery(uint32_t pageId, uint32_t pageSize, const std::string& orderBy, ProjectFilter filter);
		const HTTPRequest &GetDownloadFileQuery(const unsigned int projectid, const std::string & pathid, ProjectFileDownloadType type);

	protected:
		RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);
	protected:
		bool        m_initialized;
		RMSystemPara m_params;
		RMTenant    m_tenant;
		unsigned int   m_userid;
		std::string    m_ticket;
		std::vector<RMProject> m_projectlist;
		HTTPRequest	   m_listProjectsQuery;		
		HTTPRequest		m_downloadFileQuery;

		friend class RMUser;
	};
}

#endif /* __RMCCORE_RESTFUL_RMPROJECT_H__*/
