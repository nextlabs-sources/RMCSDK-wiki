//
//  UploadTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 4/16/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class UploadTestTests: XCTestCase {
    
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
    
    func testUploadManager() {
        //uploadManager has SDMLAsyncActionManager and session info(sessionID, tenant, user, dbmanager, uploadmanager...)
        let uploadManager = self.session?.getUploadManager()
        print("upload manager is~~~~~: \(uploadManager!.value)")
        XCTAssertNil(uploadManager?.error)
    }
    
    var nxlfile: SDMLNXLFileInterface?
    
    func testUploadFile() {
        let uploadManager = self.session?.getUploadManager()
        let cacheManager = self.session?.getCacheManager().value as! SDMLCacheManagerInterface
        let fileName = "json1-2018-07-11-14-45-10.txt"
        let getFile = cacheManager.searchFile(containName: fileName).value
        print(getFile)
        
        for file in getFile!{
            let nxlfile = file as? SDMLNXLFileInterface
            print(nxlfile!.getName())
            let expect = expectation(description: "upload")
            //upload file and return a string which contains baseAddress, owner...
            let str = uploadManager?.value?.upload(file: nxlfile!, progress: { (_) in
            }) { (result) in
                //sleep for some time to wait log file be uploaded to server and deleted
                sleep(10)
                //result is the uploaded NXL file
                expect.fulfill()
                guard let uploadedFile = result.value else{
                    print("---- upload file failed: \(result.error) ----")
                    XCTAssertNil(result.error)
                    return
                }
                print(result.value?.getName())
                print("---- upload file successfully \(uploadedFile) ----")
            }
            //if need cancel upload, just enable below code
//            let cancelResult = uploadManager?.value?.cancel(id: str!)
//            XCTAssertTrue(true)
//            print("--- cancel: \(cancelResult) ----")
            waitForExpectations(timeout: Constant.timeout){ (error) in
            }
            //if need to delete file, just enable below code
//            let delete = cacheManager.deleteFile(file: file)
//            print(delete.value)
//            let checkfile = cacheManager.searchFile(containName: fileName).value
//            print(checkfile?.count)
//            XCTAssertEqual(checkfile?.count, 0)
        }
    }

    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}

extension UploadTestTests {
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
