//
//  rmmydrive.h
//  rmccore
//
//  Created by Haobo Wang on 4/8/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMMYDRIVE_H__
#define __RMCCORE_RESTFUL_RMMYDRIVE_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/network/httpClient.h"
#include "rmccore/restful/rmsyspara.h"
#include "rmccore/restful/rmtenant.h"

namespace RMCCORE {
    class RMMyDrive : public RestfulBase
    {
    public:
        RMMyDrive();
        ~RMMyDrive();
        
        bool IsInitialized(void){return m_initialized;}
    public:
        uint64_t GetUsage(void) const { return m_usage; }
        uint64_t GetQuota(void) const { return m_totalquota; }
		uint64_t GetVaultUsage(void) const { return m_vaultusage; }
		uint64_t GetVaultQuota(void) const { return m_vaultquota; }

    public: //function inherit from RestfulBase
        RetValue ImportFromJson(JsonObject * value);
        RetValue ImportFromString(std::string jsonstr);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
        
        RetValue ImportFromRMSResponse(JsonObject * root);
        RetValue ImportFromRMSResponse(std::string jsonstr);
    public://MyDrive API
        const HTTPRequest &GetStorageQuery(void);
        
    protected:
        RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);
    protected:
        bool        m_initialized;
        
        RMSystemPara m_params;
        RMTenant    m_tenant;
        uint64_t    m_usage;
        uint64_t    m_totalquota;
		uint64_t    m_vaultusage;
		uint64_t    m_vaultquota;
        
        unsigned int   m_userid;
        std::string    m_ticket;
        
        HTTPRequest    m_MyDriveStorageQuery;
        
        friend class RMUser;
    };
}

typedef struct _MYVAULT_FILE_INFO {
	std::string  m_pathid;
	std::string  m_pathdisplay;
	std::string  m_repoid;
	std::string  m_duid;
	std::string  m_nxlname;
	uint64_t     m_lastmodified;
	uint64_t     m_creationtime;
	uint64_t     m_sharedon;
	std::string  m_sharedwith;
	uint64_t	 m_size;
	bool         m_bdeleted;
	bool         m_brevoked;
	bool         m_bshared;

	std::string	 m_sourcerepotype;
	std::string	 m_sourcefilepathdisplay;
	std::string	 m_sourcefilepathid;
	std::string	 m_sourcereponame;
	std::string	 m_sourcerepoid;
} MYVAULT_FILE_INFO;

namespace RMCCORE {
	class RMMyVaultFile : public RestfulBase
	{
	public:
		RMMyVaultFile();
		~RMMyVaultFile();

	public:
		std::string GetPathId(void) const { return m_pathid; }
		std::string GetPathDisplay(void) const { return m_pathdisplay; }
		std::string GetRepoId(void) const { return m_repoid; }
		std::string GetDuid(void) const { return m_duid; }
		std::string GetFileName(void) const { return m_nxlname; }
		uint64_t	GetLastModified(void) const { return m_lastmodified; }
		uint64_t	GetCreatedTime(void) const { return m_creationtime; }
		uint64_t	GetSharedTime(void) const { return m_sharedon; }
		std::string GetSharedWith(void) const { return m_sharedwith; }
		uint64_t	GetFileSize(void) const { return m_size; }
		bool		IsDeleted(void) const { return m_bdeleted; }
		bool		IsRevoked(void) const { return m_brevoked; }
		bool		IsShared(void) const { return m_bshared; }

		std::string GetSourceRepoType(void) const { return m_sourcerepotype; }
		std::string GetSourceFilePathDisplay(void) const { return m_sourcefilepathdisplay; }
		std::string GetSourceFilePathId(void) const { return m_sourcefilepathid; }
		std::string GetSourceRepoName(void) const { return m_sourcereponame; }
		std::string GetSourceRepoId(void) const { return m_sourcerepoid; }

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
	protected:
		std::string  m_pathid;
		std::string  m_pathdisplay;
		std::string  m_repoid;
		std::string  m_duid;
		std::string  m_nxlname;
		uint64_t     m_lastmodified;
		uint64_t     m_creationtime;
		uint64_t     m_sharedon;
		std::string  m_sharedwith;
		uint64_t	 m_size;
		bool         m_bdeleted;
		bool         m_brevoked;
		bool         m_bshared;

		std::string	 m_sourcerepotype;
		std::string	 m_sourcefilepathdisplay;
		std::string	 m_sourcefilepathid;
		std::string	 m_sourcereponame;
		std::string	 m_sourcerepoid;
	};

	class RMMyVault : public RestfulBase
	{
	public:
		RMMyVault();
		~RMMyVault();

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
		std::vector<RMMyVaultFile>* GetMyVaultFiles() { return &m_myvaultfiles; };
		std::vector<MYVAULT_FILE_INFO>* GetMyVaultFileInfos() { return &m_myvaultfileinfos; };

		const HTTPRequest &GetMyVaultFilesQuery(uint32_t pageId, uint32_t pageSize, const std::string& orderBy, const std::string& searchString);
		const HTTPRequest &GetMyVaultDownloadFileQuery(const std::string & pathid, uint32_t downloadtype);

	protected:
		RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);

	protected:
		bool		 m_initialized;
		RMSystemPara m_params;
		RMTenant     m_tenant;
		unsigned int   m_userid;
		std::string    m_ticket;

		uint64_t       m_servertime;
		uint64_t	   m_totalfiles;
		std::vector<RMMyVaultFile> m_myvaultfiles;
		std::vector<MYVAULT_FILE_INFO> m_myvaultfileinfos;

		HTTPRequest    m_MyVaultFilesQuery;
		HTTPRequest    m_MyVaultDownloadFileQuery;

		friend class RMUser;
	};
}

typedef struct _SHAREDWITHME_FILE_INFO {
	std::string  m_duid;
	std::string  m_nxlname;
	std::string  m_filetype;
	uint64_t	 m_size;
	uint64_t     m_shareddate;
	std::string  m_sharedby;
	std::string  m_transactionid;
	std::string  m_transactioncode;
	std::string  m_sharedlink;
	std::string  m_rights;
	std::string  m_comments;
	bool         m_isowner;
} SHAREDWITHME_FILE_INFO;

namespace RMCCORE {
	class RMSharedWithMeFile : public RestfulBase
	{
	public:
		RMSharedWithMeFile();
		~RMSharedWithMeFile();

	public:
		std::string GetDuid(void) const { return m_duid; }
		std::string GetFileName(void) const { return m_nxlname; }
		std::string GetFileType(void) const { return m_filetype; }
		uint64_t	GetFileSize(void) const { return m_size; }
		uint64_t	GetSharedDate(void) const { return m_shareddate; }
		std::string GetSharedBy(void) const { return m_sharedby; }
		std::string GetTransactionId(void) const { return m_transactionid; }
		std::string GetTransactionCode(void) const { return m_transactioncode; }
		std::string GetSharedLink(void) const { return m_sharedlink; }
		std::string GetRights(void) const { return m_rights; }
		std::string GetComments(void) const { return m_comments; }
		bool		IsOwner(void) const { return m_isowner; }

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
	protected:
		std::string  m_duid;
		std::string  m_nxlname;
		std::string  m_filetype;
		uint64_t	 m_size;
		uint64_t     m_shareddate;
		std::string  m_sharedby;
		std::string  m_transactionid;
		std::string  m_transactioncode;
		std::string  m_sharedlink;
		std::string  m_rights;
		std::string  m_comments;
		bool         m_isowner;
	};

	class RMSharedWithMe : public RestfulBase
	{
	public:
		RMSharedWithMe();
		~RMSharedWithMe();

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);
		std::vector<RMSharedWithMeFile>* GetSharedWithMeFiles() { return &m_sharedwithmefiles; };
		std::vector<SHAREDWITHME_FILE_INFO>* GetSharedWithMeFileInfos() { return &m_sharedwithmefileinfos; };

		const HTTPRequest &GetSharedWithMeFilesQuery(uint32_t pageId, uint32_t pageSize, const std::string& orderBy, const std::string& searchString);
		const HTTPRequest &GetSharedWithMeDownloadFileQuery(const std::string & transactionCode, const std::string & transactionId, bool bviewonly);
		const HTTPRequest &GetSharedWithMeReShareQuery(const std::string & transactionid, const std::string & transactioncode, const std::string & emaillist);

	protected:
		RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);

	protected:
		bool		 m_initialized;
		RMSystemPara m_params;
		RMTenant     m_tenant;
		unsigned int   m_userid;
		std::string    m_ticket;

		uint64_t       m_servertime;
		uint64_t	   m_totalfiles;
		std::vector<RMSharedWithMeFile> m_sharedwithmefiles;
		std::vector<SHAREDWITHME_FILE_INFO> m_sharedwithmefileinfos;

		HTTPRequest    m_SharedWithMeFilesQuery;
		HTTPRequest    m_SharedWithMeDownloadFileQuery;
		HTTPRequest    m_SharedWithMeReShareQuery;

		friend class RMUser;
	};
}
#endif /* __RMCCORE_RESTFUL_RMMYDRIVE_H__*/
