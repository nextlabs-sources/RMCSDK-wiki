#pragma once
#include "SDLInc.h"
#include "stdafx.h"

size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream);
bool GetTenantResponseData(const wstring router);
bool GetMyDriveInfoResponseData(uint64_t &result_code, uint64_t &usage, uint64_t &totalquota, uint64_t &vaultusage, uint64_t &vaultquota);
bool GetUpdateUserInfo(uint64_t &result_code, string &display_name);
bool GetFileInfoResponseData(string file_name);
bool GetMyVaultFileListResponseData(string page, string size, string file_name, string order_by, string filter);
bool GetUpdateRecipientsResponseData(string file_name, vector<std::string> addrecipients, vector<std::string> removerecipients);
bool GetActivityLogResponseData(string file_name, string start, string count, string orderBy, string orderByReverse);
bool GetFileTokenResponseData(string file_name);
bool GetHeartBeatResponseData();
bool GetProjectListResponseData(string page, string size, string orderby, string owedbyme);
bool GetProjectFilelistResponseData(string proid, string page, string size, string orderby, string pathid, string searchstring);
bool GetProjectFileMetadataResponseData(string projectid, string filepathid);
bool GetShareWithMeListResponseData(string page, string size, string orderby);


bool Project_Create(string user_jsonfile, string client_id, string rmsurl, string projectName, std::vector<std::string> members, string outputProject);
bool Project_UserPendingInvitations(string user_jsonfile, string client_id, string rmsurl, string outputJsonPath);
bool Project_AcceptInvitation(string user_jsonfile, string client_id, string rmsurl, string PendingInvitations_json, int projectID);
bool Project_AcceptInvitation1(int pro_id);

bool Project_MemberListResponseData(string pro_id);
bool Project_InviteMemberResponseData(string pro_id, string invite_email);
bool InvokeSharedFileResponseData(string duid);
bool DeleteMyvaultFile(string duid, string pathid);
bool AdminLoginResponseData(string userNanme, string password, string id, string clientid);
bool ListProjectAdminResponseData(string tenantid);
bool TenantListResponseData();
bool AddRemoveTenantAminResponseData(string tenant_name, bool is_add, string email);
bool AddRemoveProjectAdminResponseData(string tenant_id, bool is_add, string email);
int PolicyEvaluationResponseData(string membership_id, string tag_name, string tag_value);
bool CheckUpgradeResponseData(string processorArch, string currentVersion);

bool GetMyVaultFileMetadataResponseData(string file_name);

bool GetPolicyModels(string user_jsonfile, string client_id, string rmsurl, string tenantId, string output_PolicyModelsInfo);
bool PolicyDeploy(string user_jsonfile, string client_id, string rmsurl, string policyModelId, string policyModelName, string tenantId, string output_Project);
bool SetClassification(string user_jsonfile, string client_id, string rmsurl, string tenantId, string output_return);

bool DeletePolicyInCC(string ccrul, string JSESSIONID, string policyId, string output_return);
bool LoginWithADUser(string rmsurl, string tenant, string clientID, string username, string userid, string output_UserJson);

