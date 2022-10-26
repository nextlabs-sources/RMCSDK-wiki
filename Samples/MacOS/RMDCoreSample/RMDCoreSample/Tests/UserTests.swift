//
//  UserTests.swift
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 31/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

import Foundation

class UserTests {
    private struct Constants {
        static let resultFileName = "RMSBasicLogin.txt"
    }
    
    // MARK: Test
    
    func testUser() {
        let user = OCUser()!
        let clientId = CommonUtil.generateClientId()
        let tenant = CommonUtil.createDefaultTenant()
        if let urlRequest = user.getLoginURLRequest(clientId, tenant: tenant) {
            CommonUtil.printURLRequest(urlRequest)
        }
        let result = try? String(contentsOfFile: Constants.resultFileName, encoding: .utf8)
//        let result = try? String.init(contentsOfFile: Constants.resultFileName)
        user.importJson(result)
        CommonUtil.printUser(user)
    }
}
