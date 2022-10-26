//
//  rmheartbeat.h
//  rmccore
//
//  Created by Haobo Wang on 4/3/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMHEARTBEAT_H__
#define __RMCCORE_RESTFUL_RMHEARTBEAT_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/policy/obligation.h"

using namespace RMCCORE::OBLIGATION;

#define DEFAULT_HEARTBEAT_FREQUENCY_VALUE	1000	/*second*/

typedef struct _CLASSIFICATION_LABELS {
	std::string   name;
	bool          allow;
} CLASSIFICATION_LABELS;

typedef struct _CLASSIFICATION_CAT {
	std::string   name;
	bool          multiSelect;	
	bool          mandatory;
	std::vector<CLASSIFICATION_LABELS> labels;
} CLASSIFICATION_CAT;

namespace RMCCORE {
    class RMPolicyConfig : public RestfulBase
    {
    public:
        RMPolicyConfig();
        ~RMPolicyConfig(){};

    
        RMPolicyConfig& operator = (const RMPolicyConfig& rhs);
    public:
        const std::string &GetPolicyBundle() const {return m_policybundle;}
        bool HasWatermarkPolicy() const { return !m_watermark.empty();}
        const Watermark &GetWatermarkConfig() const {return m_watermark;}
        uint64_t GetPolicyLastModify() const {return m_lastmodify;}
        uint64_t GetPolicyBundleTimeStamp() const {return m_policytimestamp;}
		std::vector<CLASSIFICATION_CAT> GetClassificationCategories() const { return m_classification; }
    public:
        RetValue ImportFromRMSResponse(JsonObject * obj);
        RetValue ImportFromRMSResponse(std::string jsonstr);
        
    public: //function inherit from RestfulBase
        RetValue ImportFromJson(JsonObject * value);
        RetValue ImportFromString(std::string jsonstr);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
    protected:
        uint64_t    m_policytimestamp;
        uint64_t    m_lastmodify;
        Watermark   m_watermark;
        std::string m_policybundle;
		std::vector<CLASSIFICATION_CAT> m_classification;
    };
    
	typedef std::unordered_map<std::string, RMPolicyConfig> policymap;

    class RMHeartbeat : public RestfulBase
    {
    public:
        RMHeartbeat();
		~RMHeartbeat() {};
    public:
        uint32_t GetFrequency(void) const { return m_hbfrequency; }
        const Watermark & GetDefaultWatermarkSetting(void) const { return m_defaultwatermark.GetWatermarkSetting(); }
        
		size_t GetPolicyConfigCount() const { return m_policyconfigs.size(); }
		const std::string GetPolicyConfigTenantName(int index) const;
		bool GetPolicyConfig(const std::string & tenantName, RMPolicyConfig & config) const;
		policymap GetAllPolicyConfigs() const { return m_policyconfigs; }

	public:
        RetValue ImportFromRMSResponse(JsonObject * root);
        RetValue ImportFromRMSResponse(std::string jsonstr);
        
    public: //function inherit from RestfulBase
        RetValue ImportFromJson(JsonObject * value);
        RetValue ImportFromString(std::string jsonstr);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
    protected:
        uint32_t			m_hbfrequency;
		WatermarkConfig     m_defaultwatermark;
        
		policymap			m_policyconfigs;
        
    };
}

#endif /* __RMCCORE_RESTFUL_RMHEARTBEAT_H__ */
