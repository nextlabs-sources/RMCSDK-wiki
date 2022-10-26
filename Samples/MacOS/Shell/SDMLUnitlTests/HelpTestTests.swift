//
//  HelpTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 6/7/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class HelpTestTests: XCTestCase {

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

//    func testHelp() {
//        let cacheManager = self.session?.getCacheManager().value
//        let files = ""
//        for file in (files?.value)!{
//            let targetFile = file as! SDMLNXLFileInterface
//            let checkFileResult = SDMLHelper.isNXLFile(path: file)
//            if checkFileResult{
//                print("--- file name: \(targetFile.getName()) ---")
//                print("---- this is NXL file ----")
//            }else{
//                print("--- file name: \(targetFile.getName()) ---")
//                print("---- this is not NXL file ----")
//                XCTAssert(false)
//            }
//        }
//    }

    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }
}

extension HelpTestTests {
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
