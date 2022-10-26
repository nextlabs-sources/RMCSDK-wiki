//
//  TenantTests.swift
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

import Foundation

class TenantTests {
    private struct Constants {
        static let router = "https://rmtest.nextlabs.solutions"
        static let tenantID = "skydrm.com"
        static let result = "{\"statusCode\":200,\"message\":\"OK\",\"serverTime\":1512505670638,\"results\":{\"server\":\"https://rmtest.nextlabs.solutions/rms\"}}"
    }
    
    // MARK: Test
    
    func testDefaultTenant() {
        let tenant = CommonUtil.createDefaultTenant()
        CommonUtil.printTenant(tenant)
    }
    
    func testCustomTenant() {
        let tenant = OCTenant(router: Constants.router, tenant: Constants.tenantID)!
        tenant.importJson(Constants.result)
        CommonUtil.printTenant(tenant)
    }
}
