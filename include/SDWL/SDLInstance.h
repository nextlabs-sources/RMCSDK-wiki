/*!
 * \file SDLInstance.h
 *
 * \author hbwang
 * \date October 2017
 *
 * the parent class to access all data exported from SDWRmcLib Library
 */
#pragma once
#include "SDLResult.h"
#include "SDLTenant.h"
#include "SDLHttpRequest.h"
#include "SDLUser.h"

typedef enum {
	FCRAutoRenameNxlFile
} FilenameCollisionResolution;

typedef enum _RPM_SAFEDIRRELATION {
	nxrmfltRelationIsSafeDir = 0,		//Is safe dir
	nxrmfltRelationAncestorOfSafeDir,	//Ancestor of safe dir
	nxrmfltRelationPosterityOfSafeDir,	//Posterity of safe dir
	nxrmfltRelationNoneOfSafeDir		//No any relation of safe dir
} RPM_SAFEDIRRELATION;

/*
Application to use rmdsdk need call instance class first to access all other classes. Application need pass the router, working folder information.
sdk will save all information, such as user, token, ... under the working folder.
*/
class ISDRmcInstance
{
public:
	ISDRmcInstance() {};
	virtual ~ISDRmcInstance() {};

public:
	virtual SDWLResult Initialize(const std::wstring &router, const std::wstring &tenant) = 0;
	/// Initialize RMCInstance Class
	/**
	 * @pre
	 *    Call API function SDWLibCreateSDRmcInstance to obtain handle of RMCInstance Class
	 * @param
	 *    router	router URL for registered tenant. if empty, default value will be used.
	 *    tenant	tenant id string. if empty, default value will be used
	 * @return
	 *    Result: return RESULT(0) when success
	 *			SDWL_CERT_NOT_READY			certificate is not installed/fail to installed
	 *			SDWL_RMS_ERRORCODE_BASE+	RMS server error
	 */
	virtual SDWLResult Initialize(const std::wstring &workingfolder, const std::wstring &router, const std::wstring &tenant) = 0;
	/// Initialize RMCInstance Class
	/**
	* @pre
	*    Call API function SDWLibCreateSDRmcInstance to obtain handle of RMCInstance Class
	* @param
	*    workingfolder	file folder string to save SDWLInstance internal data files. if no working folder is set
	*					the data files will be saved at temporary folder set at SDWLibCreateSDRmcInstance() 
	*    router			router URL for registered tenant. if empty, default value will be used.
	*    tenant			tenant id string. if empty, default value will be used
	* @return
	*    Result: return RESULT(0) when success
	*			SDWL_PATH_NOT_FOUND			invalid working folder
	*			SDWL_CERT_NOT_READY			certificate is not installed/fail to installed
	*			SDWL_RMS_ERRORCODE_BASE+	RMS server error
	*/

	virtual SDWLResult IsInitFinished(bool& finished) = 0;
	/// Check if RMCInstance Class has finished initialization
	/**
	 * @pre
	 *    Initialize() function need be called before calling this funciton
	 * @param
	 *    bool			on success, true if initialization has finished, false if initialization still in progess.
	 * @return
	 *    Result: return RESULT(0) when success
	 */

	virtual SDWLResult CheckSoftwareUpdate(std::string &newVersionStr, std::string &downloadURL, std::string &checksum) = 0;
	/// Check if there is a different (either newer or older) version of the software on the server for the current tenant that can be downloaded for updating
	/**
	 * @pre
	 *    SDWLibCreateSDRmcInstance() function needs to be called with productName, productMajorVer, productMinorVer, and productBuild
	 *    IsInitFinished() function need be called before calling this funciton
	 * @param
	 *    newVersionStr	return version string of the software on the server, if any
	 *    downloadURL	return URL for downloading the software, if any
	 *    checksum		return SHA1 checksum of the download package, if any
	 * @return
	 *    Result: return RESULT(0) when the checking is successful (It does not necessarily mean that there is a version available for downloaded.)
	 */

	virtual SDWLResult Save(const std::wstring &folder = L"") = 0;
	/// Save SDWLInstance internal data files to specified folder
	/**
	 * @pre
	 *    IsInitFinished() function need be called before calling this funciton
	 * @param 
	 *    folder		file folder string to save SDWLInstance internal data files. if no working folder is set
	 *					the data files will be saved at temporary folder set at SDWLibCreateSDRmcInstance().
	 *					if empty, the workingfolder or temporary folder will be used.
	 *					Note: if no working folder is set, only tenant related information will be saved.
	 * @return
	 *			SDWL_PATH_NOT_FOUND			invalid working folder
	 */

	virtual SDWLResult GetCurrentTenant(ISDRmTenant ** ptenant) = 0;
	/// Get Current Tenant information
	/**
	* @pre
	*    IsInitFinished() function need be called before calling this funciton
	* @param
	*    ptenant		pointer to current using tenant class
	* @return
	*			SDWL_NOT_FOUND				initialize tenant failed. check the return code of Initialize() for more detail
	*/

	virtual SDWLResult GetLoginRequest(ISDRmHttpRequest **prequest) = 0;
	/// Get user login http request information based on current Tenant
	/**
	* @pre
	*    IsInitFinished() function need be called before calling this funciton
	* @param
	*    prequest		pointer to SkyDRM User Login http request information
	* @return
	*			SDWL_INVALID_DATA			invalid tenant information. check return message or return code of Initialize() for more detail
	*			
	*/

	virtual SDWLResult SetLoginResult(const std::string &jsonreturn, ISDRmUser **puser) = 0;
	/// Set Json return from User Login Request
	/**
	 * @pre
	 *    IsInitFinished() function need be called before calling this funciton
	 * @param
	 *	  jsonreturn	Returned JSON string from SkyDRM user login http request
	 *    puser			pointer to User interface 
	 * @return
	 *			SDWL_INVALID_DATA			invalid tenant information. check return message or return code of Initialize() for more detail
	 *          SDWL_INVALID_JSON_FORMAT	invalid json string
	 *			SDWL_RMS_ERRORCODE_BASE+	RMS server error	 *
	 */

	virtual SDWLResult GetLoginUser(const std::string &useremail, const std::string &passcode, ISDRmUser **puser) = 0;
	/// Get previous login user session
	/**
	* @pre
	*    IsInitFinished() function need be called before calling this funciton
	* @param
	*	  useremail		user email string of login user
	*     passcode		passcode assigned to login user account
	*    puser			pointer to User interface
	* @return
	*			SDWL_INVALID_DATA			invalid user email or passcode
	*           SDWL_LOGIN_REQUIRED			User session timeout, need login again
	*/

	virtual SDWLResult RPMGetLoginUser(const std::string &passcode, ISDRmUser **puser) = 0;
	/// Get previous login user session
	/**
	* @pre
	*    IsInitFinished() function need be called before calling this funciton
	* @param
	*     passcode		passcode assigned to plugin
	*    puser			pointer to User interface
	* @return
	*			SDWL_INVALID_DATA			invalid user email or passcode
	*           SDWL_LOGIN_REQUIRED			User session timeout, need login again
	*/

	virtual SDWLResult SyncUserAttributes() = 0;
	/**
	* @pre
	*    SetLoginResult() function need be called before calling this funciton
	*   
	* @return
	*			SDWL_ACCESS_DENIED			RPM driver is not ready
	*           SDWL_LOGIN_REQUIRED			User session timeout, need login again
	*/
	
	virtual SDWLResult SetRPMCachedToken() = 0;
	/**
	* @pre
	*    SetLoginResult()/SetRPMLoginResult function need be called before calling this funciton
	*
	* @return
	*			SDWL_ACCESS_DENIED			RPM driver is not ready
	*           SDWL_LOGIN_REQUIRED			User session timeout, need login again
	*/

	virtual SDWLResult CacheRPMFileToken(const std::wstring &filePath) = 0;
	/**
	* @pre
	*    SetLoginResult()/SetRPMLoginResult function need be called before calling this funciton
	*
	* @return
	*			SDWL_ACCESS_DENIED			RPM driver is not ready
	*           SDWL_LOGIN_REQUIRED			User session timeout, need login again
	*/

	virtual bool IsRPMDriverExist() = 0;
	/// Check if RPM driver is installed
	/**
	* @return
	*			return true when driver is installed, otherwise return false
	*/

	virtual SDWLResult AddRPMDir(const std::wstring &filepath,
								 FilenameCollisionResolution resolution = FCRAutoRenameNxlFile,
								 std::vector<std::pair<std::wstring, std::wstring>> *renamedFiles = NULL) = 0;
	/// Add directory for RPM
	/**
	* @param
	*	  filepath		directory
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RemoveRPMDir(const std::wstring &filepath) = 0;
	/// Remove directory for RPM
	/**
	* @param
	*	  filepath		directory
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult SetRPMClientId(const std::string &clientid) = 0;
	/// Set clientid for RPM
	/**
	* @param
	*	  clientid		client id
	* @return
	*			SDWL_INVALID_DATA			invalid client id
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult SetRPMLoginResult(const std::string &jsonreturn) = 0;
	/// Send login data to RPM
	/**
	* @pre
	*     must call SetRPMClientId() first
	*
	* @param
	*	  jsonreturn		login data
	* @return
	*			SDWL_INVALID_DATA			invalid data
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMLogout() = 0;
	/// logout RPM
	/**
	* 
	* @return
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult SetRPMPolicyBundle() = 0;
	/// Send policy bundle to RPM
	/**
	*
	* @return
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult SetRPMServiceStop(bool enable=true) = 0;
	/// enable or disable RPM stop service
	/**
	*
	* @param
	*	  enable		stop service enable (true) or disable (false)
	* @return
	*			SDWL_INVALID_DATA			invalid data
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual const std::string  GetLoginData() = 0;
	/// Get previous login data
	/**
	*
	* @return
	*			std::string         String is not empty if success.
	*/

	virtual SDWLResult SetRPMDeleteCacheToken() = 0;
	/// Delete RPM cache token
	/**
	*
	* @return
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMDeleteFileToken(const std::wstring &filename) = 0;
	/**
	* @pre
	*    SetLoginResult()/SetRPMLoginResult function need be called before calling this funciton
	*
	* @return
	*			SDWL_ACCESS_DENIED			RPM driver is not ready
	*           SDWL_LOGIN_REQUIRED			User session timeout, need login again
	*/

	virtual SDWLResult RPMRegisterApp(const std::wstring &appPath, const std::string &security) = 0;
	/// Register an application that is controlled by RMX
	/**
	* @param
	*    appPath    full path to the .exe file of the application
	*    security   security string for authentication
	* @return
	*           SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMUnregisterApp(const std::wstring &appPath, const std::string &security) = 0;
	/// Unregister an application that is controlled by RMX
	/**
	* @pre
	*    RPMRegisterApp() function needs to be called to register the application
	* @param
	*    appPath    full path to the .exe file of the application
	*    security   security string for authentication
	* @return
	*           SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMNotifyRMXStatus(bool running, const std::string &security) = 0;
	/// Notify RPM of the status of the RMX in the caller process
	/**
	* @pre
	*    RPMRegisterApp() function needs to be called to register the .exe image file of the caller process
	* @param
	*    running    true if RMX is running (caller process will become trusted)
	*               false if RMX is not runnint (caller process will become untrusted)
	*    security   security string for authentication
	* @return
	*           SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMAddTrustedProcess(unsigned long processId, const std::string &security) = 0;
	/// Add a process to trusted process list
	/**
	* @pre
	*    RPMRegisterApp() function needs to be called to register the .exe image file of the caller process.
	*    RPMNotifyRMXStatus() function needs to be called to indicated that RMX is running in the caller process.
	* @param
	*    processId  ID of process to be added
	*    security   security string for authentication
	* @return
	*           SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMRemoveTrustedProcess(unsigned long processId, const std::string &security) = 0;
	/// Remove a process from trusted process list
	/**
	* @pre
	*    RPMRegisterApp() function needs to be called to register the .exe image file of the caller process.
	*    RPMNotifyRMXStatus() function needs to be called to indicated that RMX is running in the caller process.
	*    RPMAddTrustedProcess function needs to be called to add the specified process to trusted process list.
	* @param
	*    processId  ID of process to be removed
	*    security   security string for authentication
	* @return
	*           SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMGetSecretDir(std::wstring &path) = 0;
	/// Get RPM Secret Directory
	/**
	* @param
	*    path          string to return Secret Directory path
	* @return
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMEditCopyFile(const std::wstring &filepath, std::wstring& destpath) = 0;
	/// Re-protect a local file
	/**
	* @param
	*    filepath			file path of the NXL file which will be edited
	*	 destpath			empty or a RPM folder path (if empty, we will use RPM Secret Directory)
	* @return
	*	 destpath			the generated plain file full path
	*			SDWL_PATH_NOT_FOUND			can't open the original file
	*           SDWL_INTERNAL_ERROR			fail to create nxl file, check return message for detail info.
	*
	*/

	virtual SDWLResult RPMEditSaveFile(const std::wstring &filepath, const std::wstring& originalNXLfilePath = L"", bool deletesource = false, uint32_t exitedit = 0) = 0;
	/// Re-protect a local file
	/**
	* @param
	*    filepath				full path to updated plain file
	*	 originalNXLfilePath	full path to the original nxl file; if empty "", we will search local mapping or check current folder
	*	 deletesource			delete source NXL file 
	*	 exitedit				0, not exit and not save
	*							1, not exit, but save
	*							2, exit and not save
	*							3 (and others), exit and save
	* @return
	*			SDWL_PATH_NOT_FOUND			can't open the original file
	*           SDWL_INTERNAL_ERROR			fail to create nxl file, check return message for detail info.
	*
	*/

	virtual SDWLResult RPMGetRights(const std::wstring& filepath, std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> &rightsAndWatermarks) = 0;
	/// Get the rights of plain file
	/**
	* @param
	*    filepath				full path to the plain file
	*	 rightsAndWatermarks				rights and watermark for the file for current user
	* @return
	*			SDWL_PATH_NOT_FOUND			can't open the original file
	*
	*/

	virtual SDWLResult IsRPMFolder(const std::wstring &filepath, unsigned int* dirstatus) = 0;
	/// Check directory is RPM folder
	/**
	* @param
	*	  filepath		directory
	*     dirstatus     directory status, see RPM_SAFEDIRRELATION
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMGetCurrentUserInfo(std::wstring& router, std::wstring& tenant, std::wstring& workingfolder, std::wstring& tempfolder, std::wstring& sdklibfolder) = 0;
	/// Get user login information, RPM must be already login
	/**
	* @param
	*	  router			router
	*	  tenant			tenant id
	*	  workingfolder		working folder
	*	  tempfolder		temp folder
	*	  sdklibfolder		sdk lib folder
	* @return
	*           0                           success
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMDeleteFile(const std::wstring &filepath) = 0;
	/// Check directory is RPM folder
	/**
	* @param
	*	  filepath		directory
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMGetFileRights(const std::wstring &filepath, std::vector<SDRmFileRight>& filerights) = 0;
	/**
	*  @param
	*	  filepath		file name
	*     rights        file rights
	*
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMGetFileStatus(const std::wstring &filename, unsigned int* dirstatus, bool* filestatus) = 0;
	/**
	*  @param
	*	  filename		file name
	*     dirstatus     file status, RPM_SAFEDIRRELATION
	*     filestatus    true: nxl file, false: non-nxl file
	*
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMSetAppRegistry(std::wstring& subkey, std::wstring& name, std::wstring& data, uint32_t op) = 0;
	/**
	*  @param
	*	  subkey		the name of the registry subkey to be opened
	*     name          the name of the value to be set
	*     data          the data to be stored
	*     op            0: set data, 1: remove name 
	*
	* @return
	*			SDWL_INVALID_DATA			invalid file path
	*			SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

	virtual SDWLResult RPMIsAppRegistered(const std::wstring &appPath, bool* registered ) = 0;
	/// Check an application is registered or not
	/**
	* @param
	*    appPath     full path to the .exe file of the application
	*    registered  application registered return true else return false: 
	* @return
	*           SDWL_CERT_NOT_READY         RPM driver/service is not ready
	*/

};

