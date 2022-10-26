//
//  rmsyspara.h
//  rmccore
//
//  Created by Haobo Wang on 1/31/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMSYSPARA_H__
#define __RMCCORE_RESTFUL_RMSYSPARA_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"
#include "rmccore/restful/rmproduct.h"

namespace RMCCORE {
    typedef enum {
        RPWindowsDesktop = 0,
        RPWindows2000,
        RPWindowsXP,
        RPWindowsVista,
        RPWindows7,
        RPWindows8,
        RPWindows81,
        RPWindows10,
        RPWindowsServer = 100,
        RPWindows2003,
        RPWindows2003R2,
        RPWindows2008,
        RPWindows2008R2,
        RPWindows2012,
        RPWindows2012R2,
        RPWindows2016,
        RPLinux = 200,
        RPMac = 300,
        RPWindowsPhone = 400,
        RPWindowsTablet = 500,
        RPIPhone = 600,
        RPIOS5,
        RPIOS6,
        RPIOS7,
        RPIOS8,
        RPIOS9,
        RPIPad = 700,
        RPAndriodPhone = 800,
        RPAndriodTablet = 900
    } RMPlatformID;
    
    typedef enum {
        RPAUnknown  = 0,
        RPAWindowsX86,
        RPAWindowsX64,
        RPAWindowsIA64
    } RMProcessorArch;
    
    class RMSystemPara : public RestfulBase
    {
    public:
		RMSystemPara();
        ~RMSystemPara();
    public:
		void SetProduct(const RMProduct & product) { m_product = product; }
        void SetDeviceID(const std::string deviceid);
        void SetPlatformID(RMPlatformID id) {m_platformid = id;}
        void SetProcessor(RMProcessorArch processor) {m_processor = processor;}
        bool SetClientID(const std::string& clientid);
        
        RMSystemPara& operator = (const RMSystemPara& rhs);

    public:
        const std::string GetClientID(void) const { return m_clientid; }
        const std::string GetDeviceID(void) const { return m_deviceid; }
        const RMPlatformID GetPlatformID(void) const {return m_platformid;}
        const RMProcessorArch GetProcessorType(void) const {return m_processor;}
        const RMProduct & GetProduct(void) const { return m_product; }
	private:
		void GenerateNewClientID(void);

    public:
        RetValue ImportFromRMSResponse(JsonObject * root){return RETMESSAGE(RMCCORE_ERROR_NOT_SUPPORTED, "ImportFromRMS is not support!");}
		RetValue ImportFromRMSResponse(std::string jsonstr) { return RETMESSAGE(RMCCORE_ERROR_NOT_SUPPORTED, "ImportFromRMS is not support!"); }

    public: //function inherit from RestfulBase
        RetValue ImportFromJson(JsonObject * value);
        RetValue ImportFromString(std::string jsonstr);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
    protected:
        std::string     m_clientid;
        std::string     m_deviceid;
        RMPlatformID    m_platformid;
        RMProcessorArch m_processor;
        
        RMProduct       m_product;
	};
}

#endif /* __RMCCORE_RESTFUL_RMSYSPARA_H__ */
