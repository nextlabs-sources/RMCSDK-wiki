//
//  centralpolicy.h
//  rmccore
//
//  Created by Allen Yuen on 1/1/18.
//  Copyright (C) 2017-2018 NextLabs, Inc.  All rights reserved.
//

#pragma once
#ifndef __RMCCORE_POLICY_CENTRALPOLICY_H__
#define __RMCCORE_POLICY_CENTRALPOLICY_H__

#include "rmccore/base/coreretval.h"

namespace RMCCORE {
    class CentralPolicy {
    public:
        CentralPolicy();
        ~CentralPolicy();

    public:
        RetValue ImportFromFile(const std::wstring &fName);

    public:
        RetValue ImportFromString(const std::wstring &str);

        const std::wstring ExportToString(void);

    protected:
        void *m_policies;
    };
}

#endif /* __RMCCORE_POLICY_CENTRALPOLICY_H__ */
