//
//  rmmembership.h
//  rmccore
//
//  Created by NextLabs Inc on 11/30/17.
//  Copyright © 2017 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMMEMBERSHIP_H__
#define __RMCCORE_RESTFUL_RMMEMBERSHIP_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/restful/rmcertificate.h"

namespace RMCCORE {
    typedef enum {
        // actual values must match RMS RESTful API spec
        MTTenant = 0,
        MTProject = 1,
        MTSystemBucket = 2
    } MembershipType;

    class RMMembership : public RestfulBase
    {
    public:
        RMMembership();
        ~RMMembership();
        
    public:
        const std::string GetID() const { return m_rmsid; }
        const std::string GetTenantID() const { return m_tenantid; }
		const std::string GetTokenGroupName() const { return m_tokenGroupName; }
        MembershipType GetType() const { return m_type; }
        unsigned int GetProjectID() const { return m_projectid;}
        
        RMMembership& operator = (const RMMembership& rhs);
                
    public:
        RetValue ImportFromString(std::string jsonstr);
        RetValue ImportFromJson(JsonObject * value);
        
        RetValue ImportFromRMSResponse(JsonObject * obj);
		RetValue ImportFromRMSResponse(std::string jsonstr);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
   
    public:
        RMCertificate   m_certificate;

    private:
        std::string     m_rmsid;
        std::string     m_tenantid;
		std::string		m_tokenGroupName;
        unsigned int    m_projectid;
        MembershipType  m_type;
    };
}

#endif /* __RMCCORE_RESTFUL_RMMEMBERSHIP_H__ */
