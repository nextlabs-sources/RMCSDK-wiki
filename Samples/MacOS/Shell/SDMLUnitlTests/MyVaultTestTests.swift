//
//  MyVaultTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 6/6/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class MyVaultTestTests: XCTestCase {

    private struct Constant {
        static let timeout: TimeInterval = 1200
        static let userResponse = RMCAPITest.Constant.rmtest
        static let httpCookie = RMCAPITest.Constant.rmtestcookie
    }
    
    var session: SDMLSessionInterface?
    
    override func setUp() {
        super.setUp()
        session = createInstance(identifier: "a").value
        
        let fetchRMSCompletion: (SDMLResult<Any?>) -> () = {
            result in
        }
        initTenant(completion: fetchRMSCompletion)
        
        let json = Constant.userResponse
        let userCookie = Constant.httpCookie
        let getCookie = RMCAPITest.getHTTPCookiesFromString(from: userCookie)
        let userLogin = self.session?.setLoginResult(cookies: getCookie!, response: json)
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }

    func testGetStorage() {
        let myVault = getMyVault()
        print("---- \(myVault) ----")
        let expect = expectation(description: "getstorage")
        myVault?.getStorage{ (result) in
            //result has usage value and quota value
            expect.fulfill()
            guard let resultInfo = result.value else{
                print("--- get my vault storage fail: \(result.error) ---")
                XCTAssertNil(result.error)
                return
            }
            if let usage = result.value?.usage, let quota = result.value?.quota {
                let usageString = ByteCountFormatter.string(fromByteCount: Int64(usage), countStyle: ByteCountFormatter.CountStyle.file)
                let quotaString = ByteCountFormatter.string(fromByteCount: Int64(quota), countStyle: ByteCountFormatter.CountStyle.file)
                print("--- usage is: \(usageString), quota is: \(quotaString) ---")
            }
        }
        waitForExpectations(timeout: Constant.timeout){ (error) in
        }
    }
    
    private func getMyVault() -> SDMLMyVaultInterface?{
        guard let myVault = self.session?.getMyVaultService().value else {
            XCTAssert(false)
            return nil
        }
        return myVault
    }

    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}

extension MyVaultTestTests {
    func initTenant(router: String = RMCAPITest.Constant.router, tenant: String = RMCAPITest.Constant.tenant, completion: @escaping (SDMLResult <Any?>) -> ()){
        let fetchRMSExpect = expectation(description: "fetch rms")
        session?.initTenant(router: router, tenant: tenant, completion: {
            (result) in
            completion(result)
            fetchRMSExpect.fulfill()
        })
        waitForExpectations(timeout: Constant.timeout) {
            (error) in
        }
    }
}
