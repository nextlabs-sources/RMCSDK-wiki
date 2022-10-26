/*!
 * \file SDLUser.h
 *
 * \author hbwang
 * \date 2017/11/14 23:05
 *
 * 
 */
#pragma once

/*
The definition is used to communicate between application and sdk library.
*/

typedef enum _USERIDPTYPE {
	SKYDRM = 0,
	SAML,
	GOOGLE,
	FACEBOOK,
	YAHOO
} USER_IDPTYPE;

typedef enum {
	RIGHT_VIEW = 1,
	RIGHT_EDIT = 2,
	RIGHT_PRINT = 4,
	RIGHT_CLIPBOARD = 8,
	RIGHT_SAVEAS = 0x10,
	RIGHT_DECRYPT = 0x20,
	RIGHT_SCREENCAPTURE = 0x40,
	RIGHT_SEND = 0x80,
	RIGHT_CLASSIFY = 0x100,
	RIGHT_SHARE = 0x200,
	RIGHT_DOWNLOAD = 0x400,
	RIGHT_WATERMARK = 0x40000000
}SDRmFileRight;

typedef struct _SDR_FILE_ACTIVITY_INFO {
	std::string duid;
	std::string email;
	std::string operation;
	std::string deviceType;
	std::string deviceId;
	std::string accessResult;
	uint64_t accessTime;
} SDR_FILE_ACTIVITY_INFO;

typedef struct _SDR_PROJECT_INFO {
	uint32_t      projid;
	std::string   name;
	std::string   displayname;
	std::string   description;
	bool          bowner;
	uint64_t	  totalfiles;
	std::string   tenantid;
} SDR_PROJECT_INFO;

typedef struct _SDR_PROJECT_FILE_INFO {
	std::string   m_fileid;
	std::string   m_duid;
	std::string   m_pathdisplay;
	std::string   m_pathid;;
	std::string   m_nxlname;
	uint64_t	  m_lastmodified;
	uint64_t	  m_created;
	uint64_t	  m_size;
	bool		  m_bfolder;
	unsigned int  m_ownerid;
	std::string	  m_ownerdisplayname;
	std::string   m_owneremail;
} SDR_PROJECT_FILE_INFO;

typedef struct _SDR_MYVAULT_FILE_INFO {
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
} SDR_MYVAULT_FILE_INFO;

typedef struct _SDR_SHAREDWITHME_FILE_INFO {
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
} SDR_SHAREDWITHME_FILE_INFO;

typedef enum _WATERMARK_ROTATION {
	NOROTATION = 0,
	CLOCKWISE,
	ANTICLOCKWISE
} WATERMARK_ROTATION;

typedef struct _SDR_WATERMARK_INFO {
	std::string   text;
	std::string   fontName;
	std::string   fontColor;
	int           fontSize;
	int           transparency;
	WATERMARK_ROTATION           rotation;
	bool          repeat;
} SDR_WATERMARK_INFO;

typedef struct _SDR_OBLIGATION_INFO {
	std::wstring                                        name;
	std::vector<std::pair<std::wstring, std::wstring>>  options;
} SDR_OBLIGATION_INFO;

typedef enum {
	RL_OProtect = 1,
	RL_OShare,
	RL_ORemoveUser,
	RL_OView,
	RL_OPrint,
	RL_ODownload,
	RL_OEdit,
	RL_ORevoke,
	RL_ODecrypt,
	RL_OCopyContent,
	RL_OCaptureScreen,
	RL_OClassify,
	RL_OReshare,
	RL_ODelete
}RM_ActivityLogOperation;

typedef enum {
	RL_RDenied = 0,
	RL_RAllowed
}RM_ActivityLogResult;

typedef enum {
	RL_ATPersonal = 0,//default
	RL_ATProject
}RM_ActivityLogAccountType;

typedef enum {
	RF_All = 0,
	RF_OwnedByMe,
	RF_OwnedByOther
} RM_ProjectFilter;

typedef enum {
	RD_Normal = 0,
	RD_ForViewer,
	RD_ForOffline
} RM_ProjectFileDownloadType;

enum IExpiryType {
	NEVEREXPIRE = 0,
	RELATIVEEXPIRE,
	ABSOLUTEEXPIRE,
	RANGEEXPIRE
};

struct SDR_Expiration {
	IExpiryType   type;
	uint64_t start;
	uint64_t end;

	SDR_Expiration() {
		type = IExpiryType::NEVEREXPIRE;
		start = 0;
		end = 0;
	}
};

typedef enum {
	RLOProtect = 1,
	RLOShare,
	RLORemoveUser,
	RLOView,
	RLOPrint,
	RLODownload,
	RLOEdit,
	RLORevoke,
	RLODecrypt,
	RLOCopyContent,
	RLOCaptureScreen,
	RLOClassify,
	RLOReshare,
	RLODelete
}SRMActivityLogOperation;

typedef struct {
	std::string   name;
	bool          allow;
} SDR_CLASSIFICATION_LABELS;

typedef struct {
	std::string   name;
	bool          multiSelect;
	bool          mandatory;
	std::vector<SDR_CLASSIFICATION_LABELS> labels;
} SDR_CLASSIFICATION_CAT;

typedef struct {
	uint64_t   startDate;
	uint64_t   endDate;
} SDR_META_DATA_VALIDITY;

struct SDR_FILE_META_DATA {
	std::string  name;
	std::string  fileLink;
	uint64_t     lastmodify;
	uint64_t     protectedon;
	uint64_t     sharedon;
	// std::vector<SDRmFileRight>  rights;
	bool         shared;
	bool         deleted;
	bool         revoked;
	uint32_t     protectionType;
	std::vector<std::string> recipients;
	SDR_Expiration expiration;

	// project file meta data
	std::string	pathDisplay;
	std::string	pathid;
	bool		owner;
	bool		nxl;
	std::string	tags;

	SDR_FILE_META_DATA() {
		lastmodify = 0;
		protectedon = 0;
		sharedon = 0;
		shared = false;
		deleted = false;
		revoked = false;
		protectionType = 0;
		owner = false;
		nxl = false;
	}
} ;


// This is not a starnder rmsdk types, special designed for c# proj to use
// by Osmond, easy outer client to use and check nxl file
// by default, client should not take any responsibilities about how nxl file works
typedef struct _SDR_NXL_FILE_FINGER_PRINT {
	std::wstring name;
	std::wstring localPath; 

	uint64_t fileSize;	
	bool isOwner;
	bool isFromMyVault;
	bool isFromProject;
	uint32_t projectId;		    // only enabled when isFromProject=true;
	bool isByAdHocPolicy;
	bool IsByCentrolPolicy;

	std::wstring tags;			// only enabled when IsByCentrolPolicy=true;
	SDR_Expiration expiration;	// only enabled when isByAdHocPolicy=true;
	std::wstring adHocWatermar;	// only enabled when isByAdHocPolicy=true;
	std::vector<SDRmFileRight> rights; // both adhoc and centrol policy

}SDR_NXL_FILE_FINGER_PRINT;