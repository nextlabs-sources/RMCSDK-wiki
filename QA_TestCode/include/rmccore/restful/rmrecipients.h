//
//  rmrecipients.h
//  rmccore
//
//  Created by Haobo Wang on 2/2/18.
//  Copyright © 2018 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMCCORE_RESTFUL_RMRECIPIENTS_H__
#define __RMCCORE_RESTFUL_RMRECIPIENTS_H__

#include "rmrestbase.h"
#include "rmccore/utils/json.h"

namespace RMCCORE {
    typedef std::vector<std::string> RMRecipientList;
    
    class RMRecipients : public RestfulBase
    {
    public:
        RMRecipients(const RMRecipientList & recipients);
        RMRecipients();
        ~RMRecipients();
    public:
        const RMRecipientList GetRecipients(void) const {return m_recipients;}
        const RMRecipientList GetAddRecipients(void) const { return m_add_recipients; }
        const RMRecipientList GetRemoveRecipients(void) const { return m_remove_recipients; }
		const std::string GetComments(void) const { return m_comments; }
        
        uint8_t AddRecipients(RMRecipientList recipients, bool buploaded = true);
        uint8_t RemoveRecipients(RMRecipientList recipients, bool buploaded = true);
    public:
        bool NeedUpdateRMS(void){ return (m_remove_recipients.size() != 0 || m_add_recipients.size() != 0);}
        RMRecipients& operator = (const RMRecipients& rhs);

		void UpdateComments(const std::string comments) { m_comments = comments; }
    public:
        RetValue ImportFromRMSResponse(JsonObject * root);
		RetValue ImportFromRMSResponse(std::string jsonstr);

    public: //function inherit from RestfulBase
        RetValue ImportFromJson(JsonObject * value);
        RetValue ImportFromString(std::string jsonstr);
        
        const std::string ExportToString(void);
        JsonValue * ExportToJson(void);
    private:
        void ResetData();
    protected:
		std::string		m_comments;
        RMRecipientList m_recipients;
        RMRecipientList m_remove_recipients;
        RMRecipientList m_add_recipients;

    };
}

#endif /* __RMCCORE_RESTFUL_RMRECIPIENTS_H__ */
