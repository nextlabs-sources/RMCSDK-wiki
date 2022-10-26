//
//  FileServiceTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 6/6/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class FileServiceTestTests: XCTestCase {

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

    func testFileService() {
        let caseFile = RMCAPITest.testReadTXTFile(casefile: "DecryptFileService")
        if caseFile.isEmpty{
            print("---- no content in case file. ----")
            XCTAssert(false)
            return
        }
        //file service value is SDML.SDMLFileService
        guard let fileService = self.session?.getNXLFileService().value else {
            XCTAssertNil(self.session?.getNXLFileService().error)
            return
        }
        
        for cases in caseFile{
            let caseNum = cases.components(separatedBy: ";").dropLast()
            let run = caseNum[1]
            if run == "yes" {
                let file = caseNum[2]
                let destinationFolder = caseNum[3]
                let expect = expectation(description: "DecryptFile")
                let overwrite = Bool.init(caseNum[4])
                fileService.decryptFile(path: file, toFolder: destinationFolder, isOverwrite: overwrite!){
                    (result) in
                    expect.fulfill()
                    guard let decryptResult = result.value else{
                        print("--- decrypt file failed: \(result.error) ---")
                        XCTAssertNil(result.error)
                        return
                    }
                    let decryptFile = result.value
                    let fileManager = FileManager.default
                    let checkFileExisted = fileManager.fileExists(atPath: decryptFile!)
                    if checkFileExisted{
                        print("--- file is correct!!! ---")
                        print("{{{case: \(caseNum[0])--- pass --- }}}")
                        print("----------------------------------------------------------")
                    }else{
                        XCTAssert(false)
                        print("FIND FILE FAILED")
                        print("{{{case: \(caseNum[0])--- share file failed --- }}}")
                        print("----------------------------------------------------------")
                    }
                }
                waitForExpectations(timeout: Constant.timeout){
                    (error) in
                }
            }else{
                print("{{{{case: \(caseNum[0])--- no need run ---}}}}")
                print("----------------------------------------------------------")
            }
        }
        
    }

    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}

extension FileServiceTestTests {
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
