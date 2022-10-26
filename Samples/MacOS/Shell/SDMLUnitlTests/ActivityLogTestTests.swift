//
//  ActivityLogTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 6/4/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class ActivityLogTestTests: XCTestCase {

    private struct Constant {
        static let timeout: TimeInterval = 1200
        static let userResponse = RMCAPITest.Constant.rmtest
        static let httpCookie = RMCAPITest.Constant.rmtestcookie
    }
    
    var session: SDMLSessionInterface?
    var logManager: SDMLLogManagerInterface?
    
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
        self.logManager = self.session?.getLogManager().value
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }

    func testgetLogManager() {
        //logManager contains session(sessionId, tenant, user),dbmanager.storage, logmanager.storage,networkmanager and so on
        let logmanager = self.session?.getLogManager()
        //logmanager value like <SDML.SDMLActivityLogManager: 0x6000000035f0>
        print("----log manager is: \(logmanager?.value)----")
        XCTAssertNil(logmanager?.error)
    }
    
    func testGetLogList() {
        let searchNXLFiles = self.session?.getCacheManager().value?.searchFile(containName: "1-2018-06-27-15-29-04.ppt.nxl").value
        if searchNXLFiles?.count == 0{
            print("---- Cannot find any file you want ----")
            XCTAssert(false)
        }
        
        for file in searchNXLFiles! {
            let nxlfile = file as? SDMLNXLFileInterface
            print(nxlfile!)
            let expect = expectation(description: "log")
            logManager?.getLogList(file: nxlfile!){
                (result) in
                expect.fulfill()
                let logs = result.value
                if logs == nil {
                    print("------ get log failed!!! -------")
                    print(result.error)
                    XCTAssertNotNil(logs)
                    return
                }
                else if logs?.count == 0{
                    print("---- no log find for this file: \(logs) ----")
                    XCTAssertGreaterThan((logs?.count)!, 0)
                }else{
                    print("------ log conut is: \(logs!.count) -------")
                    for log in logs!{
                        print("---- file name: \(log.getName()) ----")
                        print("---- access time: \(log.getAccessTime()) ----")
                        print("---- file DUID: \(log.getDUID()) ----")
                        print("---- operation: \(log.getOperation()) ----")
                        print("---- shared email: \(log.getEmail()) ----")
                        print("---- isallowed?: \(log.isAllowed()) ----")
                    }
                }
            }
            waitForExpectations(timeout: Constant.timeout){
                (error) in
            }
        }
    }
    
    func testAddLog() {
        
    }

    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}

extension ActivityLogTestTests {
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
