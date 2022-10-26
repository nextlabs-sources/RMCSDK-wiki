#pragma once
/*!
* \class rmprojectfile
*
* \brief main class for project file
*
* \author stsai
* \date May 2018
*/
#pragma once
#ifndef __RMCCORE_RESTFUL_RMPROJECTFILES_H__
#define __RMCCORE_RESTFUL_RMPROJECTFILES_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/network/httpClient.h"
#include "rmccore/restful/rmsyspara.h"
#include "rmccore/restful/rmtenant.h"
typedef struct _PROJECT_FILE_INFO {
	std::string   m_fileid;
	std::string   m_duid;
	std::string   m_nxlname;
	std::string   m_pathdisplay;
	std::string   m_pathid;;
	uint64_t	  m_lastmodified;
	uint64_t	  m_created;
	uint64_t	  m_size;
	bool		  m_bfolder;
	unsigned int  m_ownerid;
	std::string	  m_ownerdisplayname;
	std::string   m_owneremail;
} PROJECT_FILE_INFO;

namespace RMCCORE {
	class RMProjectFile : public RestfulBase
	{
	public:
		RMProjectFile();
		~RMProjectFile();

	public:
		unsigned int GetUserID(void) const { return m_userid; }
		std::string GetDisplayName(void) const { return m_displayname; }
		std::string GetUserEmail(void) const { return m_owneremail; }
		std::string GetFileName(void) const { return m_nxlname; }
		std::string GetPathId(void) const { return m_pathid; }
		std::string GetFileId(void) const { return m_fileid; }
		std::string GetDuid(void) const { return m_duid; }
		std::string GetPathDisplay(void) const { return m_pathdisplay; }
		std::string GetName(void) const { return m_username; }
		uint64_t	GetLastModified(void) const { return m_lastmodified; }
		uint64_t	GetCreatedTime(void) const { return m_creationtime; }
		uint64_t	GetFileSize(void) const { return m_size; }
		bool		IsFolder(void) const { return m_folder; }

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
	protected:
		std::string  m_fileid;
		std::string  m_duid;
		std::string  m_pathdisplay;
		std::string  m_pathid;;
		std::string  m_username;  // displayName
		std::string  m_nxlname;
		uint64_t     m_lastmodified;
		uint64_t     m_creationtime;

		uint64_t	m_size;
		bool        m_folder;
		unsigned int	m_userid;
		std::string m_displayname;
		std::string m_owneremail;

	};

	class RMProjectFiles : public RestfulBase
	{
	public:
		RMProjectFiles();
		~RMProjectFiles();

	protected:
		RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
		std::vector<std::string>* GetListFiles() { return &m_listfiles; };
		std::vector<RMProjectFile>* GetProjectFile() { return &m_projfiles; };
		std::vector<PROJECT_FILE_INFO>* GetProjectFileInfo() { return &m_fileinfo; };

	protected:
		bool           m_initialized;
		RMSystemPara   m_params;
		RMTenant       m_tenant;
		unsigned int   m_userid;
		std::string    m_ticket;
		uint64_t       m_servertime;
		uint64_t       m_usage;
		uint64_t       m_totalquota;
		uint64_t       m_lastUpdatedTime;
		uint64_t	   m_totalfiles;
		std::vector<RMProjectFile> m_projfiles;
		std::vector<std::string> m_listfiles;
		std::vector<PROJECT_FILE_INFO> m_fileinfo;
	};

}

#endif /* __RMCCORE_RESTFUL_RMPROJECTFILES_H__*/

