//
//  rmrestbase.h
//  rmccore
//
//  Created by Haobo Wang on 11/29/17.
//  Copyright © 2017 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMRESTBASE_H__
#define __RMCCORE_RESTFUL_RMRESTBASE_H__

#include <string>

#include "rmccore/base/coreretval.h"
#include "rmccore/utils/json.h"

namespace RMCCORE {
    class RestfulBase
    {
    public:
        RestfulBase() {};
        virtual ~RestfulBase() {};
        
    public:
        virtual RetValue ImportFromString(std::string jsonstr) = 0;
        virtual RetValue ImportFromJson(JsonObject * value) = 0;
        
        virtual const std::string ExportToString(void) = 0;
        virtual JsonValue * ExportToJson(void) = 0;
        
        virtual RetValue ImportFromRMSResponse(JsonObject * root) = 0;
		virtual RetValue ImportFromRMSResponse(std::string jsonstr) = 0;
    };
    
}

#endif /* __RMCCORE_RESTFUL_RMRESTBASE_H__ */
