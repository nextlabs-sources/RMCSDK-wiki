//
//  rmcertificate.h
//  rmccore
//
//  Created by Haobo Wang on 1/26/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMCERTIFICATE_H__
#define __RMCCORE_RESTFUL_RMCERTIFICATE_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"

namespace RMCCORE {
    typedef std::vector<uint8_t> RMAgreement;
    
    class RMCertificate : public RestfulBase
    {
    public:
        RMCertificate();
        ~RMCertificate();
        
    public:
        RMCertificate& operator = (const RMCertificate& rhs);
        
        const RMAgreement GetAgreement0() const { return m_agreement0;}
        const RMAgreement GetAgreement1() const { return m_agreement1;}
        
    public:
        RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);

    public:
        RetValue ImportFromString(std::string jsonstr);
        RetValue ImportFromJson(JsonObject * value);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
        
    private:
        RMAgreement     m_agreement0;
        RMAgreement     m_agreement1;
    };
}

#endif /* __RMCCORE_RESTFUL_RMCERTIFICATE_H__ */
