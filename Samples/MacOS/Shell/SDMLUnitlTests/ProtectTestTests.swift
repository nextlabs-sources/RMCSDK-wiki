//
//  ProtectTestTests.swift
//  ProtectTestTests
//
//  Created by mini on 4/4/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class ProtectTestTests: XCTestCase {
    
    private struct Constant{
        static let timeout: TimeInterval = 360
        static let serverResponse = RMCAPITest.Constant.rmtest
        static let httpCookie = RMCAPITest.Constant.rmtestcookie
    }
    //initialize the session value
    var session: SDMLSessionInterface?
    
    override func setUp() {
        super.setUp()
        //create an instance, the identifier can be any string or blank
        //run fetchRMS to initialization router and tenant by initTenant()
        session = createInstance(identifier: "a").value
        let fetchRMSCompletion: (SDMLResult<Any?>) -> () = {
            result in
        }
        initTenant(completion: fetchRMSCompletion)
        //set login user json file and get http cookie reponse from server, then set user login
        let json = Constant.serverResponse
        let cookieResponse = Constant.httpCookie
        let getCookie = RMCAPITest.getHTTPCookiesFromString(from: cookieResponse)
        let userLogin = self.session?.setLoginResult(cookies: getCookie!, response: json)
      //following code is using to set shell app clientGUID as the same with RMCAPITest app, then can get token successfully, just need to run one time
//        let txtfile = Bundle.main.path(forResource: "sysValue", ofType: "txt")
//        let sys = try? String.init(contentsOfFile: txtfile!)
//        print(sys!)
//        UserDefaults.standard.set(sys, forKey: "SystemParameter")
    }
    
    override func tearDown() {
        super.tearDown()
        //logout user after finish case
        //        self.session?.logout(completion: {result in
        //        })
    }
    
    func testProtectManager() {
        //protectManager contains session(sessionId, tenant, user) and some other info
        let manager = self.session?.getProtectManager()
        print("The protect manager is: \(manager!.value)")
        //if get protectManager successfully, then the value should be SDML.SDMLProtectManager
        //if get failed, throw out the reason
        XCTAssertNil(manager?.error)
    }
    
    func testProtectFile() {
        //read all cases from ProtectTestTests.txt, one element is one case from this Array
        var caseFile = RMCAPITest.testReadTXTFile(casefile: "ProtectTestTests")
        if caseFile.isEmpty{
            print("no content in case file")
            XCTAssert(false)
            return
        }
        for cases in caseFile{
            //get all elements from one case, one case is one Array, then one elements is a parameter for protectOriginFile function
            let caseNum = cases.components(separatedBy: ";")
            let run = caseNum[1]
            if run == "yes"{
                let filePath = caseNum[2]
                let rights = RMCAPITest.getRights(fileRight: caseNum[3])
                let fileWatermark = SDMLWatermark.init(text: caseNum[4])
                let manager = self.session?.getProtectManager().value
                //the protectOriginFile is a asynchronous function, set expectation and time out
                let expect = expectation(description: "protect")
                manager?.protectOriginFile(path: filePath, rights: rights, watermark: fileWatermark, expiry: nil) {
                    //get the result which is the encrypted NXL file
                    (result) in
                    expect.fulfill()
                    guard let file = result.value else{
                        print("case: \(caseNum[0]) print file error: \(result.error)")
                        XCTAssertNil(result.error)
//                        XCTAssertThrowsError(result.error, "getTokenfailed")
                        print("{{{case: \(caseNum[0])--- protect file failed ---}}}")
                        print("----------------------------------------------------------")
                        return
                    }
                    //get the NXL file full path
                    let nxlfilePath = "/Users/mini/Library/Containers/com.nxrmc.Shell/Data/Library/Application Support/com.nxrmc.Shell/rmtest.nextlabs.solutions/skydrm.com/36/cache/" + file.getName()
                    let fileManager = FileManager.default
                    let nxlFileRights = file.getFileRights()
                    //judgement the nxlfile generates in cache folder
                    //check file if existed or not
                    let checkFileExisted = fileManager.fileExists(atPath: nxlfilePath)
                    //check nxlFileSize>originalFileSize or not
                    let checkFileSize = RMCAPITest.checkFileSize(nxlFile: nxlfilePath, originalFile: caseNum[2])
                    //check NXLFileRights is the same with expect
                    let checkFileRights = RMCAPITest.checkFileRights(nxlFileRights: nxlFileRights, fileRight: caseNum[3])
                    if checkFileExisted && checkFileSize && checkFileRights {
                        print("--- file is correct!!! ---")
                        print("{{{case: \(caseNum[0])--- pass ---}}}")
                        print("----------------------------------------------------------")
                    }else{
                        XCTAssert(false)
                        print("FIND FILE FAILED")
                        print("{{{case: \(caseNum[0])--- protect file failed ---}}}")
                        print("----------------------------------------------------------")
                    }
                }
                waitForExpectations(timeout: Constant.timeout) { (error) in
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

extension ProtectTestTests {
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
