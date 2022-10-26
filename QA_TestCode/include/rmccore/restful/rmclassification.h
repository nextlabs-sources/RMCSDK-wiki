#pragma once
#ifndef __RMCCORE_RESTFUL_CLASSIFICATION_H__
#define __RMCCORE_RESTFUL_CLASSIFICATION_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/network/httpClient.h"
#include "rmccore/restful/rmsyspara.h"
#include "rmccore/restful/rmtenant.h"
#include "rmccore/restful/rmprojectfile.h"
#include "rmccore/restful/rmheartbeat.h"

namespace RMCCORE {
	class RMClassification : public RestfulBase
	{
	public:
		RMClassification();
		~RMClassification();

		bool IsInitialized(void) { return m_initialized; }

	public: //function inherit from RestfulBase
		RetValue ImportFromJson(JsonObject * value);
		RetValue ImportFromString(std::string jsonstr);

		const std::string ExportToString(void);
		JsonValue * ExportToJson(void);

		RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);

		int GetMaxCategoryNum() const { return m_maxCategoryNum; }
		int GetMaxLabelNum() const { return m_maxLabelNum; }
		std::vector<CLASSIFICATION_CAT> GetClassificationCategories() const { return m_classification; }
		const HTTPRequest &GetClassificationQuery(const std::string tenantid);

	//protected:
		RetValue Initialize(const RMSystemPara &sys, const RMTenant & tenant, unsigned int userid, const std::string ticket);

	protected:
		bool          m_initialized;
		RMSystemPara  m_params;
		RMTenant      m_tenant;
		unsigned int  m_userid;
		std::string   m_ticket;

		HTTPRequest	   m_ClassificationQuery;
		uint64_t	m_servertime;
		int         m_maxCategoryNum;
		int         m_maxLabelNum;
		std::vector<CLASSIFICATION_CAT> m_classification;

	};
}

#endif /* __RMCCORE_RESTFUL_CLASSIFICATION_H__*/