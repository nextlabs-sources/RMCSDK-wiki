// RMCAPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <unordered_map>
#include <fstream>
#include "SDLInc.h"



// This dummy string needs to be replaced with the real security string which is available from NextLabs.
std::string security = "{00000000-0000-0000-0000-000000000000}";



// Test directory and test files.
const std::wstring testDir(L"C:\\test");
const std::wstring testFile1(L"file1.txt");
const std::wstring testFile2(L"file2.txt");
const std::wstring testFile3(L"file3.txt");
const std::wstring testFile4(L"file4.txt");
const std::wstring testFile5(L"file5.txt");
const std::wstring testFile6(L"file6.txt");
const std::wstring testFile7(L"file7.txt");
const std::wstring testFile8(L"file8.txt");



void PrintError(const std::wstring &fn, const SDWLResult &res)
{
    std::wcout << fn << L" failed, Error: " << res.ToString() << std::endl;
} // PrintError

std::string TimeStr(uint64_t unixTimeInMs)
{
    const __time64_t unixTime = unixTimeInMs / 1000;
    const unsigned short unixTimeMs = unixTimeInMs % 1000;
    std::tm time;
    _gmtime64_s(&time, &unixTime);

    // Construct timestamp string "YYYY/MM/DD HH:MM:SS.mmm UTC".
    // 1. Construct "YYYY/MM/DD HH:MM:SS" (19 chars).
    // 2. Append ".mmm UTC" (8 chars).
    char timeStr[19 + 8 + 1];
    std::strftime(timeStr, 19 + 1, "%Y/%m/%d %H:%M:%S", &time);
    sprintf_s(&timeStr[19], 8 + 1, ".%03hu UTC", unixTimeMs);

    return timeStr;
} // TimeStr

SDWLResult HandleLogin(ISDRmcInstance *&pInstance, ISDRmTenant *&pTenant, ISDRmUser *&pUser)
{
    SDWLResult res;

    //
    // Check if user is already logged in.
    //
    res = RPMGetCurrentLoggedInUser(security, pInstance, pTenant, pUser);
    if (res.GetCode() == SDWL_SUCCESS) {
        // User already logged in.  Done.
        return res;
    }
    if (res.GetCode() != SDWL_LOGIN_REQUIRED) {
        // Some other error.
        PrintError(L"SDWLibCreateInstance", res);
        return res;
    }

    //
    // Use not logged in.  Ask RPM to display the login dialog, and launch this app again after login finishes.
    //
    ISDRmcInstance *pInstance2;
    res = SDWLibCreateInstance(&pInstance2);
    if (!res) {
        PrintError(L"SDWLibCreateInstance", res);
        return res;

    }

    WCHAR appPath[MAX_PATH];
    const std::wstring appArgs = L"";
    GetModuleFileNameW(NULL, appPath, _countof(appPath));
    res = pInstance2->RPMRequestLogin(appPath, appArgs);
    if (!res) {
        PrintError(L"RPMRequestLogin", res);
        return res;
    }

    //
    // Done.  Tell our caller to exit.
    //
    SDWLibDeleteRmcInstance(pInstance2);
    return RESULT(SDWL_LOGIN_REQUIRED);
} // HandleLogin

void PrintUserInfo(ISDRmUser *pUser, ISDRmTenant *pTenant)
{
    std::wcout << L"User name:\t" << pUser->GetName() << std::endl;
    std::wcout << L"User email:\t" << pUser->GetEmail() << std::endl;
    std::wcout << L"User ID:\t" << pUser->GetUserID() << std::endl;
    std::wcout << L"User IDP type:\t" << pUser->GetIdpType() << std::endl;
    std::wcout << std::endl;

    std::wcout << L"Tenant:\t\t" << pTenant->GetTenant() << std::endl;
    std::wcout << L"Router URL:\t" << pTenant->GetRouterURL() << std::endl;
    std::wcout << L"RMS URL:\t" << pTenant->GetRMSURL() << std::endl;
    std::wcout << std::endl;
} // PrintUserInfo

SDWLResult GetProjectsInfoFromRMS(ISDRmUser *pUser, size_t &numProjects)
{
    SDWLResult res;

    // Tell the ISDRmUser object to get the project list from RMS.
    res = pUser->GetListProjects(1, 1000, "name", RF_All);
    if (!res) {
        PrintError(L"GetListProjects", res);
        return res;
    }

    // Print the projects in the project list in ISDRmuser object.
    const std::vector<SDR_PROJECT_INFO> &projectsInfo = pUser->GetProjectsInfo();
    numProjects = projectsInfo.size();
    std::cout << "Number of projects:\t" << numProjects << std::endl;

    for (const auto &projectInfo : projectsInfo) {
        std::cout << "{" << std::endl;
        std::cout << "\tID:\t\t\t" << projectInfo.projid << std::endl;
        std::cout << "\tname:\t\t\t" << projectInfo.name << std::endl;
        std::cout << "\tdisplay name:\t\t" << projectInfo.displayname << std::endl;
        std::cout << "\tdescription:\t\t" << projectInfo.description << std::endl;
        std::cout << "\towner:\t\t\t" << projectInfo.bowner << std::endl;
        std::cout << "\ttotal files:\t\t" << projectInfo.totalfiles << std::endl;
        std::cout << "\tparent tenant ID:\t" << projectInfo.parenttenantid << std::endl;
        std::cout << "\tparent tenant name:\t" << projectInfo.parenttenantid << std::endl;
        std::cout << "\ttoken group name:\t" << projectInfo.tokengroupname << std::endl;
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    return RESULT(SDWL_SUCCESS);
} // GetProjectsInfoFromRMS

SDWLResult GetProjectFilesInfoFromRMS(ISDRmUser *pUser, size_t projectIndex)
{
    SDWLResult res;

    // Get project info.
    const SDR_PROJECT_INFO &projectInfo = pUser->GetProjectsInfo()[projectIndex];

    // Tell the ISDRmUser object to get the project file list from RMS.
    std::vector<SDR_PROJECT_FILE_INFO> filesInfo;
    res = pUser->GetProjectListFiles(projectInfo.projid, 1, 1000, "name", "/", "", filesInfo);
    if (!res) {
        PrintError(L"GetProjectListFiles", res);
        return res;
    }

    // Print the files in the file list.
    std::cout << "Number of files in project " << projectInfo.name << ":\t" << filesInfo.size() << std::endl;

    for (const auto &fileInfo : filesInfo) {
        std::cout << "{" << std::endl;
        std::cout << "\tfile ID:\t\t" << fileInfo.m_fileid << std::endl;
        std::cout << "\tDUID:\t\t\t" << fileInfo.m_duid << std::endl;
        std::cout << "\tdisplay path:\t\t" << fileInfo.m_pathdisplay << std::endl;
        std::cout << "\tpath ID:\t\t" << fileInfo.m_pathid << std::endl;
        std::cout << "\tNXL name:\t\t" << fileInfo.m_nxlname << std::endl;
        std::cout << "\tlast modified:\t\t" << TimeStr(fileInfo.m_lastmodified) << std::endl;
        std::cout << "\tcreated:\t\t" << TimeStr(fileInfo.m_created) << std::endl;
        std::cout << "\tsize:\t\t\t" << fileInfo.m_size << std::endl;
        std::cout << "\tfolder:\t\t\t" << fileInfo.m_bfolder << std::endl;
        std::cout << "\towner ID:\t\t" << fileInfo.m_ownerid << std::endl;
        std::cout << "\towner display name:\t" << fileInfo.m_ownerdisplayname << std::endl;
        std::cout << "\towner email:\t\t" << fileInfo.m_owneremail << std::endl;
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    return RESULT(SDWL_SUCCESS);
} // GetProjectfilesInfoFromRMS

SDWLResult GetWorkspaceFilesInfoFromRMS(ISDRmUser *pUser)
{
    SDWLResult res;

    // Tell the ISDRmUser object to get the Workspace file list from RMS.
    std::vector<SDR_WORKSPACE_FILE_INFO> filesInfo;
    res = pUser->GetWorkspaceFiles(1, 1000, "/", "name", "", filesInfo);
    if (!res) {
        PrintError(L"GetWorkspaceFiles", res);
        return res;
    }

    // Print the files in the file list.
    std::cout << "Number of files in Workspace:\t" << filesInfo.size() << std::endl;

    for (const auto &fileInfo : filesInfo) {
        std::cout << "{" << std::endl;
        std::cout << "\tfile ID:\t\t" << fileInfo.m_fileid << std::endl;
        std::cout << "\tDUID:\t\t\t" << fileInfo.m_duid << std::endl;
        std::cout << "\tdisplay path:\t\t" << fileInfo.m_pathdisplay << std::endl;
        std::cout << "\tpath ID:\t\t" << fileInfo.m_pathid << std::endl;
        std::cout << "\tNXL name:\t\t" << fileInfo.m_nxlname << std::endl;
        std::cout << "\tfile type:\t\t" << fileInfo.m_filetype << std::endl;
        std::cout << "\tlast modified:\t\t" << TimeStr(fileInfo.m_lastmodified) << std::endl;
        std::cout << "\tcreated:\t\t" << TimeStr(fileInfo.m_created) << std::endl;
        std::cout << "\tsize:\t\t\t" << fileInfo.m_size << std::endl;
        std::cout << "\tfolder:\t\t\t" << fileInfo.m_bfolder << std::endl;
        std::cout << "\towner ID:\t\t" << fileInfo.m_ownerid << std::endl;
        std::cout << "\towner display name:\t" << fileInfo.m_ownerdisplayname << std::endl;
        std::cout << "\towner email:\t\t" << fileInfo.m_owneremail << std::endl;
        std::cout << "\tmodified by:\t\t" << fileInfo.m_modifiedby << std::endl;
        std::cout << "\tmodified by name:\t" << fileInfo.m_modifiedbyname << std::endl;
        std::cout << "\tmodified by email:\t" << fileInfo.m_modifiedbyemail << std::endl;
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    return RESULT(SDWL_SUCCESS);
} // GetWorkspaceFilesInfoFromRMS

SDWLResult GetMyDriveFilesInfoFromRMS(ISDRmUser *pUser)
{
    SDWLResult res;

    // Tell the ISDRmUser object to get the MyDrive file list from RMS.
    std::vector<SDR_MYDRIVE_FILE_INFO> filesInfo;
    res = pUser->MyDriveListFiles(L"/", filesInfo);
    if (!res) {
        PrintError(L"MyDriveListFiles", res);
        return res;
    }

    // Print the files in the file list.
    std::cout << "Number of files in MyDrive:\t" << filesInfo.size() << std::endl;

    for (const auto &fileInfo : filesInfo) {
        std::cout << "{" << std::endl;
        std::cout << "\tpath ID:\t" << fileInfo.m_pathid << std::endl;
        std::cout << "\tdisplay path:\t" << fileInfo.m_pathdisplay << std::endl;
        std::cout << "\tname:\t\t" << fileInfo.m_name << std::endl;
        std::cout << "\tlast modified:\t" << TimeStr(fileInfo.m_lastmodified) << std::endl;
        std::cout << "\tsize:\t\t" << fileInfo.m_size << std::endl;
        std::cout << "\tfolder:\t\t" << fileInfo.m_bfolder << std::endl;
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    return RESULT(SDWL_SUCCESS);
} // GetMyDriveFilesInfoFromRMS

SDWLResult GetMyVaultFilesInfoFromRMS(ISDRmUser *pUser)
{
    SDWLResult res;

    // Tell the ISDRmUser object to get the MyVault file list from RMS.
    std::vector<SDR_MYVAULT_FILE_INFO> filesInfo;
    res = pUser->GetMyVaultFiles(1, 1000, "fileName", "", filesInfo);
    if (!res) {
        PrintError(L"GetMyVaultFiles", res);
        return res;
    }

    // Print the files in the file list.
    std::cout << "Number of files in MyVault:\t" << filesInfo.size() << std::endl;

    for (const auto &fileInfo : filesInfo) {
        std::cout << "{" << std::endl;
        std::cout << "\tpath ID:\t\t\t" << fileInfo.m_pathid << std::endl;
        std::cout << "\tdisplay path:\t\t\t" << fileInfo.m_pathdisplay << std::endl;
        std::cout << "\trepo ID:\t\t\t" << fileInfo.m_repoid << std::endl;
        std::cout << "\tDUID:\t\t\t\t" << fileInfo.m_duid << std::endl;
        std::cout << "\tNXL name:\t\t\t" << fileInfo.m_nxlname << std::endl;
        std::cout << "\tlast modified:\t\t\t" << TimeStr(fileInfo.m_lastmodified) << std::endl;
        std::cout << "\tcreation time:\t\t\t" << TimeStr(fileInfo.m_creationtime) << std::endl;
        std::cout << "\tshared on:\t\t\t" << TimeStr(fileInfo.m_sharedon) << std::endl;
        std::cout << "\tshared with:\t\t\t" << fileInfo.m_sharedwith << std::endl;
        std::cout << "\tsize:\t\t\t\t" << fileInfo.m_size << std::endl;
        std::cout << "\tdeleted:\t\t\t" << fileInfo.m_bdeleted << std::endl;
        std::cout << "\trevoked:\t\t\t" << fileInfo.m_brevoked << std::endl;
        std::cout << "\tshared:\t\t\t\t" << fileInfo.m_bshared << std::endl;
        std::cout << "\tsource repo type:\t\t" << fileInfo.m_sourcerepotype << std::endl;
        std::cout << "\tsource file display path:\t" << fileInfo.m_sourcefilepathdisplay << std::endl;
        std::cout << "\tsource file path ID:\t\t" << fileInfo.m_sourcefilepathid << std::endl;
        std::cout << "\tsource repo name:\t\t" << fileInfo.m_sourcereponame << std::endl;
        std::cout << "\tsource repo ID:\t\t\t" << fileInfo.m_sourcerepoid << std::endl;
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    return RESULT(SDWL_SUCCESS);
} // GetMyVaultFilesInfoFromRMS

SDWLResult ProtectFileForProject(ISDRmUser *pUser, const std::wstring &originalFilePath, bool useAdHocPolicy,
                                 size_t projectIndex, std::wstring &nxlFilePath)
{
    SDWLResult res;

    // Get project info.
    const SDR_PROJECT_INFO &projectInfo = pUser->GetProjectsInfo()[projectIndex];

    // Use the original file's directory as the NXL file's directory.
    nxlFilePath = originalFilePath;
    nxlFilePath.erase(nxlFilePath.rfind(L'\\'));

    std::vector<SDRmFileRight> rights;
    SDR_WATERMARK_INFO watermarkInfo;
    SDR_Expiration expire;
    std::string tags;

    if (useAdHocPolicy) {
        // Use Ad-Hoc policy.  Need to specify Ad-Hoc rights, watermark, and expiry info.
        rights.push_back(RIGHT_VIEW);
        rights.push_back(RIGHT_EDIT);
        rights.push_back(RIGHT_DOWNLOAD);
        rights.push_back(RIGHT_WATERMARK);
        watermarkInfo.text = "$(User)$(Date)$(Time)";
        watermarkInfo.fontName = "";
        watermarkInfo.fontColor = "";
        watermarkInfo.fontSize = 20;
        watermarkInfo.transparency = 2;
        watermarkInfo.rotation = NOROTATION;
        watermarkInfo.repeat = true;
        expire.type = IExpiryType::NEVEREXPIRE;
    } else {
        // Use Central policy.  Need to specify attribute names and values that will be used during Central policy evaluation.
        //
        // In this example, the attributes for this file are:
        // Key1: Value1             (single-value attribute)
        // Key2: Value2A, Value2B   (multi-value attribute)
        //
        // The attributes are specified in a JSON string encoded in UTF-8.
        tags = u8"{\"Key1\":[\"Value1\"],\"Key2\":[\"Value2A\",\"Value2B\"]}";
    }

    const std::string membershipId = pUser->GetMembershipID(projectInfo.projid);

    std::wcout << L"Protecting " << originalFilePath;
    std::cout << " for project " << projectInfo.name;
    std::cout << " with " << (useAdHocPolicy ? "Ad-Hoc" : "Central") << " policy" << std::endl;
    res = pUser->ProtectFile(originalFilePath, nxlFilePath, rights, watermarkInfo, expire, tags, membershipId);
    if (!res) {
        PrintError(L"ProtectFile", res);
        return res;
    }

    std::wcout << "New protected file:\t" << nxlFilePath << std::endl;

    return RESULT(SDWL_SUCCESS);
} // ProtectFileForProject

SDWLResult ProtectFileForWorkspace(ISDRmUser *pUser, const std::wstring &originalFilePath, bool useAdHocPolicy,
                                   std::wstring &nxlFilePath)
{
    SDWLResult res;

    // Get System Project ID.
    bool adHoc;
    int heartbeat;
    int sysProjectId;
    std::string sysProjectTenantId;
    res = pUser->GetTenantPreference(&adHoc, &heartbeat, &sysProjectId, sysProjectTenantId);
    if (!res) {
        PrintError(L"GetTenantPreference", res);
        return res;
    }

    // Use the original file's directory as the NXL file's directory.
    nxlFilePath = originalFilePath;
    nxlFilePath.erase(nxlFilePath.rfind(L'\\'));

    std::vector<SDRmFileRight> rights;
    SDR_WATERMARK_INFO watermarkInfo;
    SDR_Expiration expire;
    std::string tags;

    if (useAdHocPolicy) {
        // Use Ad-Hoc policy.  Need to specify Ad-Hoc rights, watermark, and expiry info.
        rights.push_back(RIGHT_VIEW);
        rights.push_back(RIGHT_EDIT);
        rights.push_back(RIGHT_DOWNLOAD);
        rights.push_back(RIGHT_WATERMARK);
        watermarkInfo.text = "$(User)$(Date)$(Time)";
        watermarkInfo.fontName = "";
        watermarkInfo.fontColor = "";
        watermarkInfo.fontSize = 20;
        watermarkInfo.transparency = 2;
        watermarkInfo.rotation = NOROTATION;
        watermarkInfo.repeat = true;
        expire.type = IExpiryType::NEVEREXPIRE;
    } else {
        // Use Central policy.  Need to specify attribute names and values that will be used during Central policy evaluation.
        //
        // In this example, the attributes for this file are:
        // Key1: Value1             (single-value attribute)
        // Key2: Value2A, Value2B   (multi-value attribute)
        //
        // The attributes are specified in a JSON string encoded in UTF-8.
        tags = u8"{\"Key1\":[\"Value1\"],\"Key2\":[\"Value2A\",\"Value2B\"]}";
    }

    const std::string membershipId = pUser->GetMembershipID(sysProjectId);

    std::wcout << L"Protecting " << originalFilePath;
    std::cout << " for Workspace";
    std::cout << " with " << (useAdHocPolicy ? "Ad-Hoc" : "Central") << " policy" << std::endl;
    res = pUser->ProtectFile(originalFilePath, nxlFilePath, rights, watermarkInfo, expire, tags, membershipId);
    if (!res) {
        PrintError(L"ProtectFile", res);
        return res;
    }

    std::wcout << "New protected file:\t" << nxlFilePath << std::endl;

    return RESULT(SDWL_SUCCESS);
} // ProtectFileForWorkspace

SDWLResult ProtectFileForMyVault(ISDRmUser *pUser, const std::wstring &originalFilePath, std::wstring &nxlFilePath)
{
    SDWLResult res;

    // Use the original file's directory as the NXL file's directory.
    nxlFilePath = originalFilePath;
    nxlFilePath.erase(nxlFilePath.rfind(L'\\'));

    std::vector<SDRmFileRight> rights;
    SDR_WATERMARK_INFO watermarkInfo;
    SDR_Expiration expire;

    // Use Ad-Hoc policy.  Need to specify Ad-Hoc rights, watermark, and expiry info.  (Central policies are not allowed.)
    rights.push_back(RIGHT_VIEW);
    rights.push_back(RIGHT_EDIT);
    rights.push_back(RIGHT_DOWNLOAD);
    rights.push_back(RIGHT_WATERMARK);
    watermarkInfo.text = "$(User)$(Date)$(Time)";
    watermarkInfo.fontName = "";
    watermarkInfo.fontColor = "";
    watermarkInfo.fontSize = 20;
    watermarkInfo.transparency = 2;
    watermarkInfo.rotation = NOROTATION;
    watermarkInfo.repeat = true;
    expire.type = IExpiryType::NEVEREXPIRE;

    std::wcout << L"Protecting " << originalFilePath;
    std::cout << " for MyVault";
    std::cout << " with Ad-Hoc policy" << std::endl;
    res = pUser->ProtectFile(originalFilePath, nxlFilePath, rights, watermarkInfo, expire);
    if (!res) {
        PrintError(L"ProtectFile", res);
        return res;
    }

    std::wcout << "New protected file:\t" << nxlFilePath << std::endl;

    return RESULT(SDWL_SUCCESS);
} // ProtectFileForMyVault

SDWLResult UploadProtectedProjectFile(ISDRmUser *pUser, const std::wstring &nxlFilePath, size_t projectIndex)
{
    SDWLResult res;

    // Get project info.
    const SDR_PROJECT_INFO &projectInfo = pUser->GetProjectsInfo()[projectIndex];

    // Open NXL File.
    ISDRmNXLFile *nxlFile;
    res = pUser->OpenFile(nxlFilePath, &nxlFile);
    if (!res) {
        PrintError(L"OpenFile", res);
        return res;
    }

    std::wcout << L"Uploading " << nxlFilePath.c_str();
    std::cout << " to project " << projectInfo.name << std::endl;

    const std::wstring destFolder = L"/";
    const int uploadType = 4;   // upload project token group file
    res = pUser->UploadProjectFile(projectInfo.projid, destFolder, nxlFile, uploadType);
    if (!res) {
        PrintError(L"UploadProjectFile", res);
        return res;
    }

    // Close NXL file.
    res = pUser->CloseFile(nxlFile);
    if (!res) {
        PrintError(L"CloseFile", res);
        return res;
    }

    return RESULT(SDWL_SUCCESS);
} // UploadProtectedProjectFile

SDWLResult UploadProtectedWorkspaceFile(ISDRmUser *pUser, const std::wstring &nxlFilePath)
{
    SDWLResult res;

    // Open NXL File.
    ISDRmNXLFile *nxlFile;
    res = pUser->OpenFile(nxlFilePath, &nxlFile);
    if (!res) {
        PrintError(L"OpenFile", res);
        return res;
    }

    std::wcout << L"Uploading " << nxlFilePath.c_str() << L" to Workspace" << std::endl;

    const std::wstring destFolder = L"/";
    const bool overwrite = true;
    res = pUser->UploadWorkspaceFile(destFolder, nxlFile, overwrite);
    if (!res) {
        PrintError(L"UploadWorkspaceFile", res);
        return res;
    }

    // Close NXL file.
    res = pUser->CloseFile(nxlFile);
    if (!res) {
        PrintError(L"CloseFile", res);
        return res;
    }

    return RESULT(SDWL_SUCCESS);
} // UploadProtectedWorkspaceFile

SDWLResult UploadUnprotectedMyDriveFile(ISDRmUser *pUser, const std::wstring filePath)
{
    SDWLResult res;

    std::wcout << L"Uploading " << filePath.c_str() << L" to MyDrive" << std::endl;

    const std::wstring destFolder = L"/";
    const bool overwrite = true;
    res = pUser->MyDriveUploadFile(filePath, destFolder, overwrite);
    if (!res) {
        PrintError(L"MyDriveUploadFile", res);
        return res;
    }

    return RESULT(SDWL_SUCCESS);
} // UploadUnprotectedMyDriveFile

SDWLResult UploadProtectedMyVaultFile(ISDRmUser *pUser, const std::wstring nxlFilePath)
{
    SDWLResult res;

    std::wcout << L"Uploading " << nxlFilePath.c_str() << L" to MyVault" << std::endl;

    res = pUser->UploadFile(nxlFilePath);
    if (!res) {
        PrintError(L"UploadFile", res);
        return res;
    }

    return RESULT(SDWL_SUCCESS);
} // UploadProtectedMyVaultFile

SDWLResult GetMyVaultFileActivityLogInfoFromRMS(ISDRmUser *pUser, size_t fileIndex, const std::wstring &tempDirPath)
{
    SDWLResult res;

    // Tell the ISDRmUser object to get the MyVault file list from RMS.
    std::vector<SDR_MYVAULT_FILE_INFO> filesInfo;
    res = pUser->GetMyVaultFiles(1, (uint32_t) fileIndex + 1, "fileName", "", filesInfo);
    if (!res) {
        PrintError(L"GetMyVaultFiles", res);
        return res;
    }

    // Download the NXL file header from RMS.
    std::string targetFileHeaderPath(tempDirPath.begin(), tempDirPath.end());
    res = pUser->MyVaultGetNxlFileHeader(filesInfo[fileIndex].m_pathid, targetFileHeaderPath);
    if (!res) {
        PrintError(L"MyVaultGetNxlFileHeader", res);
        return res;
    }

    // Open the NXL file header.
    const std::wstring targetFileHeaderPathW(targetFileHeaderPath.begin(), targetFileHeaderPath.end());
    ISDRmNXLFile *nxlFile;
    res = pUser->OpenFile(targetFileHeaderPathW, &nxlFile);
    if (!res) {
        PrintError(L"OpenFile", res);
        return res;
    }

    // Tell the ISDRmUser object to get the activity log from RMS.
    std::cout << "Getting activity log of " << filesInfo[fileIndex].m_nxlname << " in MyVault" << std::endl;
    res = pUser->GetNXLFileActivityLog(nxlFile, 0, 1000, RL_SFEmpty, "", RL_OBAccessTime, false);
    if (!res) {
        PrintError(L"GetNXLFileActivityLog", res);
        return res;
    }

    // Get the activity log.
    std::vector<SDR_FILE_ACTIVITY_INFO> activitiesInfo;
    pUser->GetActivityInfo(nxlFile->GetFileName(), activitiesInfo);
    if (!res) {
        PrintError(L"GetActivityInfo", res);
        return res;
    }

    // Close the NXL file header.
    res = pUser->CloseFile(nxlFile);
    if (!res) {
        PrintError(L"CloseFile", res);
        return res;
    }

    // Print the activities in the activity log
    std::cout << "Number of activities in activity log:\t" << activitiesInfo.size() << std::endl;

    for (const auto &activityInfo : activitiesInfo) {
        std::cout << "{" << std::endl;
        std::cout << "\tDUID:\t\t" << activityInfo.duid << std::endl;
        std::cout << "\temail:\t\t" << activityInfo.email << std::endl;
        std::cout << "\toperation:\t" << activityInfo.operation << std::endl;
        std::cout << "\tdevice type:\t" << activityInfo.deviceType << std::endl;
        std::cout << "\tdevice ID:\t" << activityInfo.deviceId << std::endl;
        std::cout << "\taccess result:\t" << activityInfo.accessResult << std::endl;
        std::cout << "\taccess time:\t" << TimeStr(activityInfo.accessTime) << std::endl;
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    return RESULT(SDWL_SUCCESS);
} // GetMyVaultFileActivityLogInfoFromRMS

SDWLResult GetFileRightsAndWatermarks(ISDRmUser *pUser, const std::wstring nxlFilePath)
{
    const std::unordered_map<SDRmFileRight, const std::string> rightsStr = {
        {RIGHT_VIEW,            "View"},
        {RIGHT_EDIT,            "Edit"},
        {RIGHT_PRINT,           "Print"},
        {RIGHT_CLIPBOARD,       "Clipboard"},
        {RIGHT_SAVEAS,          "SaveAs"},
        {RIGHT_DECRYPT,         "Decrypt"},
        {RIGHT_SCREENCAPTURE,   "ScreenCapture"},
        {RIGHT_SEND,            "Send"},
        {RIGHT_CLASSIFY,        "Classify"},
        {RIGHT_SHARE,           "Share"},
        {RIGHT_DOWNLOAD,        "Download"}
    };
    SDWLResult res;

    std::wcout << L"Getting file rights and associated watermarks for non-owners" << std::endl;

    const bool doOwnerCheck = false;
    std::vector<std::pair<SDRmFileRight, std::vector<SDR_WATERMARK_INFO>>> rightsAndWatermarks;
    res = pUser->GetRights(nxlFilePath, rightsAndWatermarks, doOwnerCheck);
    if (!res) {
        PrintError(L"GetRights", res);
        return res;
    }

    for (const auto &rightAndWatermarks : rightsAndWatermarks) {
        std::cout << "{" << std::endl;
        std::cout << "\tright:\t" << rightsStr.at(rightAndWatermarks.first) << std::endl;

        std::cout << "\twatermarks:" << std::endl;
        std::cout << "\t{" << std::endl;
        for (const auto &watermark : rightAndWatermarks.second) {
            std::cout << "\t\t{" << std::endl;
            std::cout << "\t\t\ttext:\t\t" << watermark.text << std::endl;
            std::cout << "\t\t\tfont name:\t" << watermark.fontName << std::endl;
            std::cout << "\t\t\tfont color:\t" << watermark.fontColor << std::endl;
            std::cout << "\t\t\tfont size:\t" << watermark.fontSize << std::endl;
            std::cout << "\t\t\ttransparency:\t" << watermark.transparency << std::endl;
            std::cout << "\t\t\trotation:\t" << (watermark.rotation == NOROTATION ? "none" :
                                                 (watermark.rotation == CLOCKWISE ? "clockwise" : "counter-clockwise"))
                      << std::endl;
            std::cout << "\t\t\trepeat:\t\t" << watermark.repeat << std::endl;
            std::cout << "\t\t}" << std::endl;
        }
        std::cout << "\t}" << std::endl;

        std::cout << "}" << std::endl;
    }

    return RESULT(SDWL_SUCCESS);
} // GetFileRightsAndWatermarks

void printByteArray(const char array[], size_t len)
{
    const int bytesPerLine = 16;

    for (size_t i = 0; i < len; i++) {
        if (i % bytesPerLine == 0) {
            printf("\t");
        }

        printf("%02hX", (unsigned short) array[i]);

        if ((i + 1) % bytesPerLine == 0 || i == len - 1) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
} // printByteArray

int ReadFileBytes(const std::wstring &filePath)
{
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::wcout << "Cannot open " << filePath << std::endl;
        return -1;
    }

    std::streamoff size = file.tellg();
    const size_t sizeMax = 50;
    if (size > sizeMax) {
        size = sizeMax;
    }
    char *memBlock = new char [(size_t) size];

    std::wcout << "Reading first " << size << " bytes in " << filePath << std::endl;
    file.seekg(0, std::ios::beg);
    file.read(memBlock, size);
    if (!file.good()) {
        std::wcout << "Cannot read " << filePath << std::endl;
        return -2;
    }

    file.close();

    std::cout << "{" << std::endl;
    printByteArray(memBlock, (size_t) size);
    std::cout << "}" << std::endl;

    delete[] memBlock;
    return 0;
} // ReadFileBytes

SDWLResult EnsureDirIsMarkedAsRPMDir(ISDRmcInstance *pInstance, const std::wstring &dirPath)
{
    SDWLResult res;

    // Check if directory is currently RPM directory
    uint32_t dirStatus;
    res = pInstance->IsRPMFolder(dirPath, &dirStatus);
    if (!res) {
        PrintError(L"IsRPMFolder", res);
        return res;
    }
    if (dirStatus & RPM_SAFEDIRRELATION_SAFE_DIR) {
        std::wcout << dirPath << L" is already an RPM directory" << std::endl;
        return RESULT(SDWL_SUCCESS);
    }

    std::wcout << L"Marking " << dirPath << L" as RPM directory" << std::endl;
    res = pInstance->AddRPMDir(dirPath);
    if (!res) {
        PrintError(L"AddRPMDir", res);
        return res;
    }

    return RESULT(SDWL_SUCCESS);
} // EnsureDirIsMarkedAsRPMDir

SDWLResult EnsureDirIsNotMarkedAsRPMDir(ISDRmcInstance *pInstance, const std::wstring &dirPath)
{
    SDWLResult res;

    // Check if directory is currently not RPM directory
    uint32_t dirStatus;
    res = pInstance->IsRPMFolder(dirPath, &dirStatus);
    if (!res) {
        PrintError(L"IsRPMFolder", res);
        return res;
    }
    if (!(dirStatus & RPM_SAFEDIRRELATION_SAFE_DIR)) {
        std::wcout << dirPath << L" is already not an RPM directory" << std::endl;
        return RESULT(SDWL_SUCCESS);
    }

    std::wcout << L"Unmarking " << dirPath << L" as RPM directory" << std::endl;
    const bool bForce = true;
    res = pInstance->RemoveRPMDir(dirPath, bForce);
    if (!res) {
        PrintError(L"RemoveRPMDir", res);
        return res;
    }

    return RESULT(SDWL_SUCCESS);
} // EnsureDirIsNotMarkedAsRPMDir

SDWLResult ListFilesInDir(const std::wstring &dirPath)
{
    WIN32_FIND_DATA findData;
    HANDLE h = FindFirstFileW((dirPath + L"\\*").c_str(), &findData);
    if (h == INVALID_HANDLE_VALUE) {
        std::cout << "FindFirstFileW failed, err=" << GetLastError() << std::endl;
        return RESULT(GetLastError());
    }

    std::wcout << L"Files in " << dirPath << L":" << std::endl;
    do {
        std::wcout << (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? L"\tdir:\t" : L"\tfile:\t")
            << findData.cFileName << std::endl;
    } while (FindNextFileW(h, &findData));
    std::cout << std::endl;

    FindClose(h);
    return RESULT(SDWL_SUCCESS);
} // ListFilesInDir

int wmain(int argc, const wchar_t *argv[])
{
    std::cout << std::boolalpha;



    ///////////////////////////////////////////////////////////
    // ******************** Initialization ********************
    ///////////////////////////////////////////////////////////
    SDWLibInit();
    const DWORD dwVersion = SDWLibGetVersion();
    std::cout << "SkyDRM Cliekt SDK Runtime Lib version: " <<
        (int) HIBYTE(HIWORD(dwVersion)) << "." <<
        (int) LOBYTE(HIWORD(dwVersion)) << "." <<
        LOWORD(dwVersion) << std::endl << std::endl;

    ISDRmcInstance *pInstance = nullptr;
    int exitCode = 0;



    do {
        ///////////////////////////////////////////////////
        // ******************** Log-in ********************
        ///////////////////////////////////////////////////
        ISDRmTenant *pTenant;
        ISDRmUser *pUser;
        SDWLResult res;

        res = HandleLogin(pInstance, pTenant, pUser);
        if (!res) {
            if (res.GetCode() == SDWL_LOGIN_REQUIRED) {
                // The user is not logged in yet.
                std::cout << "Please log in to SkyDRM first." << std::endl;
                exitCode = 0;   // This is not an error.
            } else {
                // Some other error.
                exitCode = 1;
            }

            break;
        }

        PrintUserInfo(pUser, pTenant);

        size_t projectIndex;
        size_t fileIndex;
        std::wstring originalFilePath;
        std::wstring nxlFilePath;
        bool useAdHocPolicy;



        /////////////////////////////////////////////////////
        // ******************** Projects ********************
        /////////////////////////////////////////////////////

        //
        // Get projects info.
        //
        size_t numProjects;
        res = GetProjectsInfoFromRMS(pUser, numProjects);
        if (!res) {
            exitCode = 2;
            break;
        }

        projectIndex = numProjects - 1;     // use the last project in the project list

        //
        // Get project files info.
        //
        res = GetProjectFilesInfoFromRMS(pUser, projectIndex);
        if (!res) {
            exitCode = 3;
            break;
        }

        //
        // Protect a file for the project with Ad-Hoc policy, and upload it.
        //
        originalFilePath = testDir + L'\\' + testFile1;
        useAdHocPolicy = true;
        res = ProtectFileForProject(pUser, originalFilePath, useAdHocPolicy, projectIndex, nxlFilePath);
        if (!res) {
            exitCode = 4;
            break;
        }

        res = UploadProtectedProjectFile(pUser, nxlFilePath, projectIndex);
        if (!res) {
            exitCode = 5;
            break;
        }

        //
        // Protect a file for the project with Central policy, and upload it.
        //
        originalFilePath = testDir + L'\\' + testFile2;
        useAdHocPolicy = false;
        res = ProtectFileForProject(pUser, originalFilePath, useAdHocPolicy, projectIndex, nxlFilePath);
        if (!res) {
            exitCode = 6;
            break;
        }

        res = UploadProtectedProjectFile(pUser, nxlFilePath, projectIndex);
        if (!res) {
            exitCode = 7;
            break;
        }

        std::cout << std::endl;



        //////////////////////////////////////////////////////
        // ******************** Workspace ********************
        //////////////////////////////////////////////////////

        //
        // Get Workspace files info.
        //
        res = GetWorkspaceFilesInfoFromRMS(pUser);
        if (!res) {
            exitCode = 8;
            break;
        }

        //
        // Protect a file for Workspace with Ad-Hoc policy, and upload it.
        //
        originalFilePath = testDir + L'\\' + testFile3;
        useAdHocPolicy = true;
        res = ProtectFileForWorkspace(pUser, originalFilePath, useAdHocPolicy, nxlFilePath);
        if (!res) {
            exitCode = 9;
            break;
        }

        res = UploadProtectedWorkspaceFile(pUser, nxlFilePath);
        if (!res) {
            exitCode = 10;
            break;
        }

        //
        // Protect a file for Workspace with Central policy, and upload it.
        //
        originalFilePath = testDir + L'\\' + testFile4;
        useAdHocPolicy = false;
        res = ProtectFileForWorkspace(pUser, originalFilePath, useAdHocPolicy, nxlFilePath);
        if (!res) {
            exitCode = 11;
            break;
        }

        res = UploadProtectedWorkspaceFile(pUser, nxlFilePath);
        if (!res) {
            exitCode = 12;
            break;
        }

        std::cout << std::endl;



        ////////////////////////////////////////////////////
        // ******************** MySpace ********************
        ////////////////////////////////////////////////////

        //
        // Get MyDrive files info.
        //
        res = GetMyDriveFilesInfoFromRMS(pUser);
        if (!res) {
            exitCode = 13;
            break;
        }

        //
        // Upload an unprotected file to MyDrive.  (Protected file are not allowed.)
        //
        originalFilePath = testDir + L'\\' + testFile5;
        res = UploadUnprotectedMyDriveFile(pUser, originalFilePath);
        if (!res) {
            exitCode = 14;
            break;
        }

        std::cout << std::endl;

        //
        // Get MyVault files info.
        //
        res = GetMyVaultFilesInfoFromRMS(pUser);
        if (!res) {
            exitCode = 15;
            break;
        }

        //
        // Protect a file for MyVault with Ad-Hoc policy, and upload it.  (Central policies are not allowed.)
        //
        originalFilePath = testDir + L'\\' + testFile6;
        res = ProtectFileForMyVault(pUser, originalFilePath, nxlFilePath);
        if (!res) {
            exitCode = 16;
            break;
        }

        res = UploadProtectedMyVaultFile(pUser, nxlFilePath);
        if (!res) {
            exitCode = 17;
            break;
        }

        std::cout << std::endl;



        /////////////////////////////////////////////////////////
        // ******************** Activity Log ********************
        /////////////////////////////////////////////////////////

        //
        // Get Activity Log info.
        //
        fileIndex = 0;      // use the first file in MyVault
        res = GetMyVaultFileActivityLogInfoFromRMS(pUser, fileIndex, testDir);
        if (!res) {
            exitCode = 18;
            break;
        }

        std::cout << std::endl;



        ///////////////////////////////////////////////////////////////////////////
        // ******************** File rights and watermark info*********************
        ///////////////////////////////////////////////////////////////////////////

        //
        // Protect a file for MyVault with Ad-Hoc policy.
        //
        originalFilePath = testDir + L'\\' + testFile7;
        res = ProtectFileForMyVault(pUser, originalFilePath, nxlFilePath);
        if (!res) {
            exitCode = 19;
            break;
        }

        //
        // Get the file rights, and the associated watermarks for each right.
        //
        res = GetFileRightsAndWatermarks(pUser, nxlFilePath);
        if (!res) {
            exitCode = 20;
            break;
        }

        std::cout << std::endl;



        ///////////////////////////////////////////////////////////////////
        // ******************** Transparent decryption ********************
        ///////////////////////////////////////////////////////////////////

        //
        // Protect a file for MyVault with Ad-Hoc policy.
        //
        originalFilePath = testDir + L'\\' + testFile8;
        res = ProtectFileForMyVault(pUser, originalFilePath, nxlFilePath);
        if (!res) {
            exitCode = 21;
            break;
        }

        std::wstring nxlFilePathWithoutNxlExt = nxlFilePath;
        nxlFilePathWithoutNxlExt.erase(nxlFilePathWithoutNxlExt.rfind(L'.'));

        //
        // Make sure that the directory of the file is not marked as RPM directory.
        //
        std::wstring dirPath = originalFilePath;
        dirPath.erase(dirPath.rfind(L'\\'));
        res = EnsureDirIsNotMarkedAsRPMDir(pInstance, dirPath);
        if (!res) {
            exitCode = 22;
            break;
        }

        //
        // List files in directory.  (It shows .nxl filename extension.)
        //
        res = ListFilesInDir(dirPath);
        if (!res) {
            exitCode = 23;
            break;
        }

        //
        // Try to read the NXL file using the file name without .nxl.  (It should fail with file-not-found error.)
        //
        int res2;
        res2 = ReadFileBytes(nxlFilePathWithoutNxlExt);
        if (res2 == 0) {
            std::wcout << L"Error: Reading " << nxlFilePathWithoutNxlExt << L" succeeded when it should not!" << std::endl;
            exitCode = 24;
            break;
        }

        //
        // Read the NXL file using the .nxl file name.  (Result is encrypted content.)
        //
        res2 = ReadFileBytes(nxlFilePath);
        if (res2 != 0) {
            exitCode = 25;
            break;
        }

        //
        // Mark the directory of the file as RPM directory so that transparent decryption occurs.
        //
        res = EnsureDirIsMarkedAsRPMDir(pInstance, dirPath);
        if (!res) {
            exitCode = 26;
            break;
        }

        //
        // List files in directory.  (It does not show .nxl filename extension.)
        //
        res = ListFilesInDir(dirPath);
        if (!res) {
            exitCode = 27;
            break;
        }
        //
        // Read the NXL file using the file name without .nxl.  (Result is decrypted content.)
        //
        res2 = ReadFileBytes(nxlFilePathWithoutNxlExt);
        if (res2 != 0) {
            exitCode = 28;
            break;
        }

        //
        // Unmark the directory of the file as RPM directory so that transparent decryption does not occur.
        //
        res = EnsureDirIsNotMarkedAsRPMDir(pInstance, dirPath);
        if (!res) {
            exitCode = 29;
            break;
        }

        std::cout << std::endl;
    } while (false);



    ////////////////////////////////////////////////////
    // ******************** Cleanup ********************
    ////////////////////////////////////////////////////
    if (pInstance != nullptr) {
        SDWLibDeleteRmcInstance(pInstance);
    }

    SDWLibCleanup();



    std::cout << "End." << std::endl;
    return exitCode;
} // wmain
