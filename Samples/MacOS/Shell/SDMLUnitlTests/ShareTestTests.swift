//
//  ShareTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 4/12/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class ShareTestTests: XCTestCase {
    
    private struct Constant {
        static let timeout: TimeInterval = 10
        static let serverRequest = RMCAPITest.Constant.rmtest
        static let httpCookie = RMCAPITest.Constant.rmtestcookie
    }
    //initialize the session value
    var session: SDMLSessionInterface?
    
    override func setUp() {
        super.setUp()
        //create an instance, the identifier can be any string or blank
        //run fetchRMS to initialization router and tenant by initTenant()
        session = createInstance(identifier: "b").value
        let fetchRMSCompletion: (SDMLResult<Any?>) -> () = {
            result in
        }
        initTenant(completion: fetchRMSCompletion)
        //set login user json file and get http cookie reponse from server, then set user login
        let json = Constant.serverRequest
        let cookieResponse = Constant.httpCookie
        let getCookie = RMCAPITest.getHTTPCookiesFromString(from: cookieResponse)
        let loginResult = self.session?.setLoginResult(cookies: getCookie!, response: json)
    }
    
    override func tearDown() {
        super.tearDown()
        //logout user after finish case
        //        self.session?.logout(completion: {result in
        //        })
    }
    
    func testShareFile() {
        //shareManager is the same with protectManager, it contains session(sessionId, tenant, user) and some other info
        //read all cases from ShareTestTests.txt, one element is one case from this Array
        let shareManager = self.session?.getProtectManager()
        let caseFile = RMCAPITest.testReadTXTFile(casefile: "ShareTestTests")
        if caseFile.isEmpty{
            print("---- no content in case file. ----s")
            XCTAssert(false)
            return
        }
        for cases in caseFile{
            //get all elements from one case, one case is one Array, then one elements is a parameter for shareOriginFile function
            let caseNum = cases.components(separatedBy: ";").dropLast()
            let run = caseNum[1]
            if run == "yes"{
                let fileSharePath = caseNum[2]
                let fileRights = RMCAPITest.getRights(fileRight: caseNum[3])
                let fileRecipients = RMCAPITest.getRecipients(emailAddress: caseNum[4])
                let fileShareComment = caseNum[5]
                let fileWatermark = SDMLWatermark.init(text: caseNum[6])
                let expect = expectation(description: "share")
                shareManager?.value?.shareOriginFile(path: fileSharePath, rights: fileRights, watermark: fileWatermark, expiry: nil, recipients: fileRecipients, comment: fileShareComment){
                    (result) in
                    expect.fulfill()
                    guard let nxlFile = result.value else{
                        XCTAssertNil(result.error)
                        print("case: \(caseNum[0]) ------ \(result.error!) -------")
//                        XCTAssertThrowsError(result.error, "commonFailed(SDML.SDMLError.CommonFailedReason.invalidEmail)")
//                        XCTAssertEqual(result.error!, commonFailed(SDML.SDMLError.CommonFailedReason.invalidEmail))
                        print("{{{case: \(caseNum[0])--- share file failed----}}}")
                        print("----------------------------------------------------------")
                        return
                    }
                    //get the NXL file full path
                    let nxlfilePath = "/Users/mini/Library/Application Support/com.nxrmc.Shell/rmtest.nextlabs.solutions/skydrm.com/36/cache/" + (nxlFile.getName())
                    let fileManager = FileManager.default
                    //judgement the nxlfile generates in cache folder
                    //check file if existed or not
                    let checkFileExisted = fileManager.fileExists(atPath: nxlfilePath)
                    //get nxlfilesize and original file size, and compare them
                    let checkFileSize = RMCAPITest.checkFileSize(nxlFile: nxlfilePath, originalFile: caseNum[2])
                    //get nxlfilerights and expect file rights, then compare
                    let nxlFileRights = nxlFile.getFileRights()
                    let checkFileRights = RMCAPITest.checkFileRights(nxlFileRights: nxlFileRights, fileRight: caseNum[3])
                    if checkFileExisted && checkFileSize && checkFileRights{
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

extension ShareTestTests {
    func initTenant(router: String = "https://rmtest.nextlabs.solutions", tenant: String = "skydrm.com", completion: @escaping (SDMLResult <Any?>) -> ()){
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


