/*!
 * \file SDLUser.h
 *
 * \author hbwang
 * \date 2017/11/14 23:05
 *
 * 
 */
#pragma once
#include "SDLResult.h"
#include "SDLFiles.h"
#include "SDLNXLFile.h"
#include "SDLTypeDef.h"
#include <string>
#include <list>

/*
Application call user class to protect, decrypt, share, upload and download files. User calss can get ad-hoc or central policy
information.
*/
class ISDRmUser	
{
public:
	ISDRmUser() { };
	virtual ~ISDRmUser() { };
public:
	virtual const std::wstring GetName() = 0;
	/// Get current login user name
	/**
	 * @return
	 *    User name string
	 */
	virtual const std::wstring GetEmail() = 0;
	/// Get current login user email address for SkyDRM
	/**
	 * @return
	 *    user email string
	 */
	virtual USER_IDPTYPE GetIdpType() = 0;
	/// Get current login user identify protocol type
	/**
	 * @return
	 *    User IDP type
	 */

	virtual SDWLResult GetMyDriveInfo(uint64_t& usage, uint64_t& totalquota, uint64_t& vaultusage, uint64_t& vaultquota) = 0;
	/// Force sync MyDrive information from RMS server
	/**
	* @param
	*    usage        storage usage
	*    totalquota   total storage available
	*    vaultusage   vault usage
	*    vaultquota   total vault available
	* @return
	*			SDWL_SUCCESS	get the current storage information.
	*           SDWL_INVALID_DATA	user not login
	*/

	virtual const std::string GetPasscode() = 0;
	/// Get current login user passcode for local login
	/**
	* @return
	*    passcode string for current user.
	*/

	virtual std::vector<SDR_PROJECT_INFO>& GetProjectsInfo() = 0;
	/// Get projects information
	/**
	* @return
	*    projects information.
	*/

	virtual const std::string GetMembershipID(uint32_t projectid) = 0;
	/// Get user's membership-id of the project
	/**
	* @param
	*    projectid
	*		1		default tenant
	* @return
	*    user's membershipid of the tenant/project.
	*/

	virtual const std::string GetMembershipID(const std::string tenantid) = 0;
	/// Get user's membership-id of the tenant
	/**
	* @param
	*    tenantid
	* @return
	*    user's membershipid of the tenant/project.
	*/

	virtual const std::string GetMembershipIDByTenantName(const std::string tenantname) = 0;
	/// Get user's membership-id of the tenant
	/**
	* @param
	*    tenantname
	* @return
	*    user's membershipid of the tenant/project.
	*/

	virtual SDWLResult UpdateUserInfo() = 0;
	/// Force sync User information from RMS server
	/**
	* @param
	*    None
	* @return
	*			SDWL_SUCCESS	synchronized the latest user information from RMS. 
	*           SDWL_RMS_ERRORCODE_BASE+	RMS server error	 
	*/
	virtual SDWLResult UpdateMyDriveInfo() = 0;
	/// Force sync MyDrive information from RMS server
	/**
	* @param
	*    None
	* @return
	*			SDWL_SUCCESS	synchronized the latest MyDrive information from RMS.
	*           SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*/

	virtual SDWLResult ProtectFile(const std::wstring &filepath, std::wstring& newcreatedfilePath, const std::vector<SDRmFileRight> &rights, const SDR_WATERMARK_INFO &watermarkinfo, const SDR_Expiration &expire, const std::string& tags = "", const std::string& memberid = "" ) = 0;
	/// Protect a local file
	/**
	* @param
	*    filepath			full path to original file
	*    rights				rights assigned to the nxl file
	*	 newcreatedfilePath full path to the created nxl file
	*    watermarkinfo		watermark structure
	*    expire				SDR_Expiration structure
	*    tags				tags
	*    memberid			membership id
	* @return
	*			SDWL_PATH_NOT_FOUND			can't open the original file
	*           SDWL_INTERNAL_ERROR			fail to create nxl file, check return message for detail info.
	*
	*/

	virtual SDWLResult GetRecipients(ISDRmNXLFile * nxlfile, std::vector<std::string> &recipientsemail, std::vector<std::string> &addrecipientsemail, std::vector<std::string> &removerecipientsemail) = 0;
	/// Get Recipients list 
	/**
	* @param
	*    nxlfile		nxl file handle
	*    recipientsemail			email list of recipients existed
	*    addrecipientsemail			email list of recipients added
	*    removerecipientsemail		email list of recipients removed
	* @return
	*
	*/

	virtual SDWLResult UpdateRecipients(ISDRmNXLFile * nxlfile, const std::vector<std::string> &addrecipientsemail) = 0;
	/// Update Recipients list 
	/**
	* @param
	*    nxlfile		nxl file handle
	*    addrecipientsemail			email list of recipients to add
	*    removerecipientsemail		email list of recipients to remove
	* @return
	*
	*/


	virtual SDWLResult UploadFile(const std::wstring &nxlFilePath, const std::wstring& sourcePath = L"", const std::wstring& recipientEmails = L"", const std::wstring& comments = L"" ) = 0;
	/// upload  local file 
	/**
	* @param
	*    nxlFilePath        full path to nxl
	*    sourcePath         the source file full path  
	*    recipientEmails    the emails for uploading shared file, use a comma delimiter if there are more than one.
	*    comments           the comments for uploading shared file 
	* @return
	*     SDWL_INVALID_DATA            invalid Parameter
	*
	*/

	virtual SDWLResult UploadProjectFile(uint32_t projectid, const std::wstring &destfolder, ISDRmNXLFile* file, bool overwrite = false) = 0;
	/// upload  project file 
	/**
	* @param
	*    projectid   projectid
	*    destfolder  dest folder
	*    file        project file to  upload
	* @return
	*     SDWL_INVALID_DATA            invalid Parameter
	*
	*/

	virtual SDWLResult CloseFile(ISDRmNXLFile * file) = 0;
	/// Open a local NXL file
	/**
	* @param
	*    file[in]		pointer to nxl file class handle.
	* @return
	*/

	virtual SDWLResult OpenFile(const std::wstring &nxlfilepath, ISDRmNXLFile ** file) = 0;
	/// Open a local NXL file
	/**
	* @param
	*    nxlfilepath		full path to nxl file
	*    file[out]		pointer to pointer fo nxl file class handle . NULL if failed.
	* @return
	*			SDWL_ACCESS_DENIED	User doesn't has right to open target file
	*           SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*/

	virtual SDWLResult LogoutUser() = 0;
	/// Logout current user
	/**
	* @param
	*    none
	* @return
	*            SDWL_INVALID_DATA            Invalid logout query return
	*           ERROR_INVALID_DATA          Invalid logout result
	*/

	virtual SDWLResult GetNXLFileActivityLog(ISDRmNXLFile * file, uint64_t startPos, uint64_t count, RM_ActivityLogSearchField searchField, const std::string &searchText, RM_ActivityLogOrderBy orderByField, bool orderByReverse) = 0;
	/// Get file activity log
	/**
	* @param
	*    file       nxl file class
	*    startPos   the first returned log on sort
	*    count      the maximum log number returned in this query
	*    searchField   field name to be searched.
	*    searchText    the search text in the searching filed above(empty means all values)
	*    orderByField  the ordered by field for return result
	*    orderByReverse flag for return result order
	* @return
	*            SDWL_INVALID_DATA          Invalid log query return
	*           ERROR_INVALID_DATA          Invalid log result
	*/

	
	virtual SDWLResult AddActivityLog(const std::wstring &fileName, RM_ActivityLogOperation op, RM_ActivityLogResult result) = 0;
	/// Add file activity information
	/**
	* @param
	*    fileName   nxl file name
	*    op		file activity information
	*    result		access result: deny or allow
	* @return
	*           SDWL_NOT_FOUND              File not found
	*/

	virtual SDWLResult AddActivityLog(ISDRmNXLFile * file, RM_ActivityLogOperation op, RM_ActivityLogResult result) = 0;
	/// Add file activity information
	/**
	* @param
	*    file   nxl file
	*    op		file activity information
	*    result		access result: deny or allow
	* @return
	*           SDWL_NOT_FOUND              File not found
	*/

	virtual SDWLResult GetActivityInfo(const std::wstring &fileName, std::vector<SDR_FILE_ACTIVITY_INFO>& info) = 0;
	/// Get file activity information
	/**
	* @param
	*    fileName   nxl file name
	*    info       file activity information
	* @return
	*           SDWL_NOT_FOUND              File not found
	*/

	virtual SDWLResult GetListProjects(uint32_t pageId, uint32_t pageSize, const std::string& orderBy, RM_ProjectFilter filter) = 0;
	/// Get projects list
	/**
	* @param
	*    pageId       Page number (start from 1)
	*    pageSize     Number of records to be returned
	*    orderBy      Pass string as "lastActionTime" or "name" 
	*    searchValue  search specific value
	*    filter       If 0, will return all projects
	* @return
	*           SDWL_INVALID_DATA          Invalid list projects query return
	*           ERROR_INVALID_DATA          Invalid list projects result
	*/

	virtual SDWLResult ProjectDownloadFile(const uint32_t projectid, const std::string& pathid, std::wstring& downloadPath, RM_ProjectFileDownloadType type) = 0;
	/// Project download file
	/**
	* @param
	*    project    project class
	*    pathid     pathid
	*    downloadPath  in/out
	*					in	targetfolder
	*					out	final saved file full path
	*    orderBy    order sequence
	*    bviewonly  view only
	* @return
	*            ERROR_INVALID_DATA          Invalid download file result
	*/

	virtual SDWLResult ProjectDownloadPartialFile(const uint32_t projectid, const std::string& pathid, std::wstring& downloadPath, RM_ProjectFileDownloadType type) = 0;
	/// Project download file
	/**
	* @param
	*    project    project class
	*    pathid     pathid
	*    downloadPath  in/out
	*					in	targetfolder
	*					out	final saved file full path - file content only include NXL file header
	*    orderBy    order sequence
	*    bviewonly  view only
	* @return
	*            ERROR_INVALID_DATA          Invalid download file result
	*/

	virtual SDWLResult GetProjectListFiles(const uint32_t projectid, uint32_t pageId, uint32_t pageSize, const std::string& orderBy, const std::string& pathId, const std::string& searchString, std::vector<SDR_PROJECT_FILE_INFO>& listfiles) = 0;
	/// Get project files
	/**
	* @param
	*    projectid    project id
	*    pageId       Page number (start from 1)
	*    pageSize     Number of records to be returned
	*    orderBy      A comma-separated list of sort keys.
	*    pathid       pathid
	*    searchString  search specific string
	*    listfiles    return files list
	* @return
	*           SDWL_INVALID_DATA          Invalid project list files query return
	*            ERROR_INVALID_DATA          Invalid project list files result
	*/

	virtual SDWLResult GetMyVaultFiles(uint32_t pageId, uint32_t pageSize, const std::string& orderBy, const std::string& searchString, std::vector<SDR_MYVAULT_FILE_INFO>& listfiles) = 0;
	/// Get project files
	/**
	* @param
	*    pageId       Page number (start from 1)
	*    pageSize     Number of records to be returned
	*    orderBy      A comma-separated list of sort keys.
	*    searchString  search specific string
	*    listfiles    return files list
	* @return
	*           SDWL_INVALID_DATA          Invalid project list files query return
	*            ERROR_INVALID_DATA          Invalid project list files result
	*/
		
	virtual SDWLResult MyVaultDownloadFile(const std::string& pathid, std::wstring& downloadPath, uint32_t downloadtype) = 0;
	/// MyVault download file
	/**
	* @param
	*    pathid     pathid
	*    downloadPath  in/out
	*					in	targetfolder
	*					out	final saved file full path
	*    downloadtype  0 normal 1 for viewer (nxl header) 2 offline
	* @return
	*            ERROR_INVALID_DATA          Invalid download file result
	*/

	virtual SDWLResult MyVaultDownloadPartialFile(const std::string& pathid, std::wstring& downloadPath, uint32_t downloadtype) = 0;
	/// MyVault download file
	/**
	* @param
	*    pathid     pathid
	*    downloadPath  in/out
	*					in	targetfolder
	*					out	final saved file full path - the file is nxl header content only
	*    downloadtype  0 normal 1 for viewer (nxl header) 2 offline
	* @return
	*            ERROR_INVALID_DATA          Invalid download file result
	*/

	virtual SDWLResult GetSharedWithMeFiles(uint32_t pageId, uint32_t pageSize, const std::string& orderBy, const std::string& searchString, std::vector<SDR_SHAREDWITHME_FILE_INFO>& listfiles) = 0;
	/// Get project files
	/**
	* @param
	*    pageId       Page number (start from 1)
	*    pageSize     Number of records to be returned
	*    orderBy      A comma-separated list of sort keys.
	*    searchString  search specific string
	*    listfiles    return files list
	* @return
	*           SDWL_INVALID_DATA          Invalid project list files query return
	*            ERROR_INVALID_DATA          Invalid project list files result
	*/

	virtual SDWLResult SharedWithMeDownloadFile(const std::wstring& transactionCode, const std::wstring& transactionId, std::wstring& targetFolder, bool forViewer) = 0;
	/// Project download file
	/**
	* @param
	*    transactionCode     transactionCode
	*    transactionId     transactionId
	*    targetFolder  in/out
	*					in	targetfolder
	*					out	final saved file full path
	*    forViewer
	* @return
	*            ERROR_INVALID_DATA          Invalid download file result
	*/

	virtual SDWLResult SharedWithMeDownloadPartialFile(const std::wstring& transactionCode, const std::wstring& transactionId, std::wstring& targetFolder, bool forViewer) = 0;
	/// ShareWithMe download file
	/**
	* @param
	*    transactionCode     transactionCode
	*    transactionId     transactionId
	*    targetFolder  in/out
	*					in	targetfolder
	*					out	final saved file full path - file content only include NXL file header
	*    forViewer
	* @return
	*            ERROR_INVALID_DATA          Invalid download file result
	*/

	virtual SDWLResult SharedWithMeReShareFile(const std::string& transactionId, const std::string& transactionCode, const std::string emaillist) = 0;
	/// ShareWithMe reshare file to others
	/**
	* @param
	*    transactionCode     transactionCode
	*    transactionId     transactionId
	*    emaillist  shared to users, e.g. "abc@hello.com, xyz@hello.com"
	* @return
	*            ERROR_INVALID_DATA          Invalid result
	*/

	virtual SDWLResult GetFingerPrintWithoutToken(const std::wstring& nxlFilePath, SDR_NXL_FILE_FINGER_PRINT& fingerPrint) = 0;
	virtual SDWLResult GetFingerPrint(const std::wstring& nxlFilePath, SDR_NXL_FILE_FINGER_PRINT& fingerPrint)=0;
	/// Get Nxl File's finger print
	/*
		special designed for c# rmsdk api stub easy to check all about nxl file inforamtions
		wrapper some stable single api into a syntetic fingprint struct
	*/

	virtual SDWLResult GetRights(const std::wstring& nxlfilepath, std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> &rightsAndWatermarks, bool doOwnerCheck = true) = 0;
	/// Get the rights granted to the current user, no matter the file is a central policy file or ad-hoc policy file
	/**
	* @param
	*    nxlfilepath            full path to NXL file
	*    rightsAndWatermarks    return rights assigned to the NXL file, and their associated watermark obligations
	* @return
	*
	*/

	virtual SDWLResult GetFileRightsFromCentralPolicies(const std::wstring& nxlFilePath, std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> &rightsAndWatermarks) = 0;
	/// Get the rights granted to the current user by central policies for the passed file.
	/**
	* @param
	*    nxlFilePath            full path to NXL file
	*    rightsAndWatermarks    return rights assigned to the NXL file, and their associated watermark obligations
	* @return
	*           SDWL_NOT_FOUND          Not policy bundle for file's tenant found
	*
	*/
	virtual SDWLResult GetFileRightsFromCentralPolicies(const std::string &tenantname, const std::string &tags, std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> &rightsAndWatermarks) = 0;
	/// Get the rights granted to the current user by central policies for the passed tags on project.
	/**
	* @param
	*    tenantname            project tenant name
	*    tags					tags json string
	*    rightsAndWatermarks    return rights assigned to the NXL file, and their associated watermark obligations
	* @return
	*           SDWL_NOT_FOUND          Not policy bundle for file's tenant found
	*
	*/
	virtual SDWLResult GetFileRightsFromCentralPolicies(const uint32_t projectid, const std::string &tags, std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> &rightsAndWatermarks) = 0;
	/// Get the rights granted to the current user by central policies for the passed tags on project.
	/**
	* @param
	*    projectid            project id
	*    tags					tags json string
	*    rightsAndWatermarks    return rights assigned to the NXL file, and their associated watermark obligations
	* @return
	*           SDWL_NOT_FOUND          Not policy bundle for file's tenant found
	*
	*/

	virtual SDWLResult GetResourceRightsFromCentralPolicies(const std::wstring& resourceName, const std::wstring& resourceType, const std::vector<std::pair<std::wstring, std::wstring>> &attrs, std::vector<std::pair<SDRmFileRight, std::vector<SDR_OBLIGATION_INFO>>> &rightsAndObligations) = 0;
	/// Get the rights granted to the current user by central policies for the passed resource.
	/**
	* @param
	*    resourceName           name of resource
	*    resourceType           type of resource (e.g. "fso", "portal")
	*    attrs                  resource attributes
	*    rightsAndObligations   return rights assigned to the resource, and their associated obligations
	* @return
	*           SDWL_NOT_FOUND          Not policy bundle for current user's tenant found
	*/

	virtual SDWLResult GetHeartBeatInfo() = 0;
	/// Get heart beat infomation
	/**
	* @param
	*    none
	* @return
	*           SDWL_INVALID_DATA          Invalid heart beat query return
	*           ERROR_INVALID_DATA          Invalid heart beat result
	*/

	virtual bool GetPolicyBundle(const std::wstring& tenantName, std::string& policyBundle) = 0;
	/// Get policy bundle
	/**
	* @param
	*    tenantName  tenant name
	*    policyBundle  policy bundle
	* @return
	*           success return true, else return false
	*/

	virtual const SDR_WATERMARK_INFO GetWaterMarkInfo() = 0;
	/// Get water mark info
	/**
	* @param
	*    none
	* @return
	*           SDR_WATERMARK_INFO structure
	*/

	virtual bool IsFileProtected(const std::wstring &filepath) = 0;
	/// Check file is protected or not
	/**
	* @param
	*    filepath  full path to file 
	* @return
	*           if it's nxl file return true, otherwise return false
	*/

	virtual SDWLResult GetClassification(const std::string &tenantid, std::vector<SDR_CLASSIFICATION_CAT>& cats) = 0;
	/// Get Classification Profile
	/**
	* @param
	*    tenantid  project's tenant-id
	* @return
	*            SDWL_INVALID_DATA          Invalid heart beat query return
	*           ERROR_INVALID_DATA          Invalid heart beat result
	*/

	//virtual SDWLResult GetFilePath(const std::wstring &filename, std::wstring &targetfilepath) = 0;
	/// Get file path from file name
	/**
	* @param
	*    filename  file name
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*            SDWL_NOT_FOUND          File is not in the working directory
	*
	*/

	virtual SDWLResult GetUserPreference(uint32_t &option, uint64_t &start, uint64_t &end, std::wstring &watermark) = 0;
	/// Update User Preference
	/**
	* @param
	*    option  out option number
	*    start   out time start date, only use in option 3
	*    end     out time end date, use in option 2, 3
	*    Watermark   out  "$(User)\n$(Date) $(Time)"
	*
	*		note for option "1" - relativeDay
	*					year + month = start (high 32bits + low 32bits)
	*					week + day = end (high 32bits + low 32bits)

	* uint32_t year, month;
	* uint64_t start = ((u64)year) << 32 | month;

	* @return
	*            200 success, the rest is http error code
	*
	*/

	virtual SDWLResult UpdateUserPreference(const uint32_t option, const uint64_t start=0, const uint64_t end=0, const std::wstring watermark = L"") = 0;
	/// Update User Preference
	/**
	* @param
	*    option  option number
	*    start   time start date, only use in option 3
	*    end     time end date, use in option 2, 3
	*    Watermark     "$(User)\n$(Date) $(Time)"
	*
	*		note for option "1" - relativeDay
	*					year + month = start (high 32bits + low 32bits)
	*					week + day = end (high 32bits + low 32bits)

	* uint32_t year, month;
	* uint64_t start = ((u64)year) << 32 | month;

	* @return
	*            200 success, the rest is http error code
	*
	*/

	virtual SDWLResult GetNXLFileMetadata(const std::wstring &nxlfilepath, const std::string& pathid, SDR_FILE_META_DATA& metadata) = 0;
	/// Get file meta data
	/**
	* @param
	*    nxlfilepath  file name
	*    pathid  file pathid
	*    metadata  output file meta data
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*            SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*
	*/

	virtual SDWLResult GetProjectFileMetadata(const std::wstring &nxlfilepath, const std::string& projectid, const std::string& pathid, SDR_FILE_META_DATA& metadata) = 0;
	/// Get file meta data
	/**
	* @param
	*    nxlfilepath  file name
	*    projectid    projectid
	*    pathid  file pathid
	*    metadata  output file meta data
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*            SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*
	*/

	virtual SDWLResult GetProjectFileMetadata(const std::string& projectid, const std::string& pathid, SDR_FILE_META_DATA& metadata) = 0;
	/// Get file meta data
	/**
	* @param
	*    projectid    projectid
	*    pathid  file pathid
	*    metadata  output file meta data
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*            SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*
	*/

	virtual uint32_t GetHeartBeatFrequency() = 0;
	/// Get client heartbeat frequency setting from RMS
	///		before calling this API, make sure the GetHeartBeatInfo is called
	/**
	* @return
	*            frequency       client frequency setting from RMS
	*
	*/

	virtual SDWLResult GetTenantPreference(bool* adhoc, int* heartbeat, int* sysprojectid, std::string &sysprojecttenantid, const std::string& tenantid = "") = 0;
	/// Get Tenant Preference
	/**
	* @param
	*    adhoc       adhoc enable or disable
	*    heartbeat   heartbeat frequency
	*	 sysprojectid	system project id
	*	 sysprojecttenantid	system project tenant id
    *    tenantid    tenant id
	*
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*            SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*
	*/

	virtual bool IsTenantAdhocEnabled() = 0;
	/// Check file is protected or not
	/**
	* @param
	*    
	* @return
	*           if tenant is adhoc enabled return true, otherwise return false
	*/

	virtual SDWLResult GetProjectFileRights(const std::string& projectid, const std::string& pathid, std::vector<SDRmFileRight>& rights) = 0;
	/// Get project file meta data
	/**
	* @param
	*    projectid    projectid
	*    pathid  file pathid
	*    rights  output file rights
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*
	*/

	virtual SDWLResult GetMyVaultFileRights(const std::string& duid, const std::string& pathid, std::vector<SDRmFileRight>& rights) = 0;
	/// Get MyVault file meta data
	/**
	* @param
	*    duid    duid
	*    pathid  file pathid
	*    rights  output file rights
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*
	*/

	virtual const std::string GetSystemProjectTenantId() = 0;
	/// Get system project tenantId
	/**
	*
	* @return
	*			system project tenantId
	*/

	virtual const std::string GetDefaultTenantId() = 0;
	/// Get user default tenantId
	/**
	*
	* @return
	*			default tenantId
	*/
	virtual const std::string GetDefaultTokenGroupName() = 0;
	/// Get token group name of default tenant
	/**
	*
	* @return
	*			token group name of default tenant
	*/

	virtual SDWLResult PDDecryptPartial(const unsigned char* in, long in_len, long offset, unsigned char* out, long* out_len, const unsigned char* header, long header_len) = 0;
	/// Decrypt partial file data
	/**
	* @param
	*    in      input buffer address
	*    in_len  input buffer length
	*    offset  starting length, must be multiple blocksize
	*    out     out buffer address
	*    out_len output buffer length
	*    header  nxl file header
	*    header_len  header buffer length
	*
	*       note for header and header_len:
	*           For Ad-Hoc Policy files:
	*                   The passed header needs to be big enough to include
	*                   both the fixed header and the FilePolicy section.
	*                   Currently this means it needs to be at least 12288
	*                   bytes.
	*           For Central Policy files:
	*                   The passed header needs to be big enough to include
	*                   the fixed header, the FilePolicy section, and the
	*                   FileTag section.  Currently this means it needs to be
	*                   at least 16384 bytes.
	*
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*
	*/

	virtual SDWLResult PDSetupHeader(const unsigned char* header, long header_len, int64_t* contentLength, unsigned int* contentOffset) = 0;
	/// Decrypt partial file data
	/**
	* @param
	*    header  nxl file header
	*    header_len  header buffer length
	*    contentLength  content length
	*    contentOffset  content offset
	*
	*       note for header and header_len:
	*           For Ad-Hoc Policy files:
	*                   The passed header needs to be big enough to include
	*                   both the fixed header and the FilePolicy section.
	*                   Currently this means it needs to be at least 12288
	*                   bytes.
	*           For Central Policy files:
	*                   The passed header needs to be big enough to include
	*                   the fixed header, the FilePolicy section, and the
	*                   FileTag section.  Currently this means it needs to be
	*                   at least 16384 bytes.
	*                   The passed header needs to be big enough to include
	*                   both the fixed header and the FilePolicy section.
	*                   Currently this means it needs to be at least 12288
	*                   bytes.
	*
	* @return
	*            SDWL_INVALID_DATA       Invalid data
	*
	*/

	virtual SDWLResult ChangeRightsOfFile(const std::wstring &filepath, const std::vector<SDRmFileRight> &rights, const SDR_WATERMARK_INFO &watermarkinfo, const SDR_Expiration &expire, const std::string& tags = "") = 0;
	/// Change the rights of the file
	/**
	* @param
	*    filepath			full path to original NXL file
	*    rights				new rights assigned to the nxl file
	*    watermarkinfo		watermark structure
	*    expire				SDR_Expiration structure
	*    tags				tags
	* @return
	*			SDWL_PATH_NOT_FOUND			can't open the original file
	*           SDWL_INTERNAL_ERROR			fail to create nxl file, check return message for detail info.
	*
	*/

	virtual bool HasAdminRights(const std::wstring &nxlfilepath) = 0;
	/// Check current user whether he has the Admin Rights on the file
	/**
	* @param
	*    nxlfilepath		full path to original NXL file
	* @return
	*			true			has the 'Admin Rights' on the file
	*           false			no Admin Rights
	*
	*/

	virtual SDWLResult UpdateNXLMetaData(const std::wstring &nxlfilepath, bool bRetry = true) = 0;
	/// Sync the file metadata to RMS
	/**
	* @param
	*    nxlfilepath		full path to NXL file
	* @return
	*			SUCCESS
	*           ERROR_INVALID_DATA
	*
	*/

	virtual SDWLResult UpdateNXLMetaData(ISDRmNXLFile* file, bool bRetry = true) = 0;
	/// Sync the file metadata to RMS
	/**
	* @param
	*    ISDRmNXLFile		file pointer of NXL file
	* @return
	*			SUCCESS			
	*           ERROR_INVALID_DATA
	*
	*/

	virtual SDWLResult ResetSourcePath(const std::wstring & nxlfilepath, const std::wstring & sourcepath) = 0;
	/// Set the source path for NXL file in case that new protect file is from a temp file
	/**
	* @param
	*    nxlfilepath		new protected NXL file path (not uploaded)
	*    sourcepath			exact source file path for the new protected file
	* @return
	*			SUCCESS
	*
	*/
	virtual SDWLResult ShareFileFromMyVault(const std::wstring &filepath, const std::vector<std::string> &recipients, const std::string &repositoryId, const std::string &fileName, const std::string &filePathId, const std::string &filePath, const std::string &comment) = 0;
	/// Share the file protected under MyVault to new recipients
	/**
	* @param
	*    filepath		NXL file path
	*    recipients		array of recipient email
	*    repositoryId	MyVault repository id
	*    fileName		fileName shown in MyVault file list
	*    filePath		filePath shown in MyVault file list
	*    filePathId		filePathId shown in MyVault file list
	*    comment		comment added to shared recipients
	* @return
	*			SUCCESS
	*
	*/

	virtual SDWLResult ChangeRightsOfProjectFile(const std::wstring &oldnxlfilepath, unsigned int projectid, const std::string &fileName, const std::string &parentPathId, const std::vector<SDRmFileRight> &rights, const SDR_WATERMARK_INFO &watermarkinfo, const SDR_Expiration &expire, const std::string& tags = "") = 0;
	/// Re-classify the central policy file stored in project
	/**
	* @param
	*    oldnxlfilepath		NXL file path
	*    projectid			project id that this file belongs
	*    fileName			file name listed in project
	*    parentPathId		folder path the file stored in project
	*    rights				new rights assigned to the nxl file
	*    watermarkinfo		watermark structure
	*    expire				SDR_Expiration structure
	*    tags				tags
	* @return
	*			SUCCESS
	*
	*/
};