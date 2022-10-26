//
//  obligation.h
//  rmccore
//
//  Created by Haobo Wang on 4/3/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_POLICY_OBLIGATION_H__
#define __RMCCORE_POLICY_OBLIGATION_H__

#include "rmccore/utils/json.h"
#include "rmccore/format/nxlpolicy.h"
#include "rmccore/restful/rmrestbase.h"

using namespace RMCCORE::NXLFMT;

namespace RMCCORE {
    namespace OBLIGATION{
        class Watermark : public Obligation, RestfulBase
        {
        public:
            enum Rotation {
                NOROTATION = 0,
                CLOCKWISE,
                ANTICLOCKWISE
            };
            Watermark(JsonObject * value = NULL);
            Watermark(const std::string &text, const std::string & fontName, const std::string &fontColor, int fontSize, int transparency, Rotation rotation, bool repeat);
            virtual ~Watermark() {}
            
            Watermark& operator = (const Watermark& rhs);
			Watermark& operator = (const Obligation & rhs);
        public:
            bool empty() const {return !m_initialized;}
            
            std::string getText() const;
            std::string getFontName() const;
            int getFontSize() const;
            std::string GetFontColor() const;
            int getTransparency() const;
            Rotation getRotation() const;
            std::string getRotationString() const;
            bool getRepeat() const;
        public:
            RetValue ImportFromRMSResponse(JsonObject * obj);
            RetValue ImportFromRMSResponse(std::string jsonstr);
            
        public: //function inherit from RestfulBase
            RetValue ImportFromJson(JsonObject * value);
            RetValue ImportFromString(std::string jsonstr);
            
            const std::string ExportToString(void);
            JsonValue * ExportToJson(void);
        private:
            bool    m_initialized;
        };
        
        class WatermarkConfig: public RestfulBase
        {
        public:
            WatermarkConfig();
            ~WatermarkConfig(){};
        public:
            const Watermark & GetWatermarkSetting() const { return m_watermark;}
            const std::string & GetSerialNo() const {return m_serialno;}
        public:
            RetValue ImportFromRMSResponse(JsonObject * obj);
            RetValue ImportFromRMSResponse(std::string jsonstr);
            
        public: //function inherit from RestfulBase
            RetValue ImportFromJson(JsonObject * value);
            RetValue ImportFromString(std::string jsonstr);
            
            const std::string ExportToString(void);
            JsonValue * ExportToJson(void);
        private:
            Watermark   m_watermark;
            std::string m_serialno;
        };
    }
}

#endif /* __RMCCORE_POLICY_OBLIGATION_H__ */
