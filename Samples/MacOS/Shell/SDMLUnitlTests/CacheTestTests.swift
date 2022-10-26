//
//  CacheTestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 4/19/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class CacheTestTests: XCTestCase {
    
    private struct Constant{
        static let timeout: TimeInterval = 10
        static let serverResponse = RMCAPITest.Constant.rmtest
        static let httpCookie = RMCAPITest.Constant.rmtestcookie
    }
    //initialize the session value, cachemanager value, nxlfile value
    var session: SDMLSessionInterface?
    var cacheMa: SDMLCacheManagerInterface?
    var files: SDMLNXLFileInterface?
    
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
        //set the cachemanager value
        self.cacheMa = self.session?.getCacheManager().value as! SDMLCacheManagerInterface
    }
    
    override func tearDown() {
        super.tearDown()
        //logout user after finish case
        //        self.session?.logout(completion: {result in
        //        })
    }
    
    func testCacheManager() {
        //cachemanager contains session(sessionId, tenant, user),dbmanager.storage, logmanager.storage,networkmanager...
        let cachemanager = self.session?.getCacheManager()
        //cache manager value should be SDML.SDMLCacheManager
        print("the cache manager is : \(cachemanager?.value)")
        XCTAssertNil(cachemanager?.error)
    }
    
    func testSearchFile() {
        let file = "1-2018-06-27-14-20-01.doc"
        let searchResult = cacheMa?.searchFile(containName: file)
        //seared file result is SDML.SDMLNXLFile, it's array
        print("can seach file: \(searchResult?.value)")
        guard let result = searchResult?.value else{
            print("cannot find file: \(searchResult?.error)")
            XCTAssertNil(searchResult?.error)
            return
        }
        //get the count of result array and compare
        if result.count == 0 {
            XCTAssert(false)
            return
        }
        XCTAssertGreaterThanOrEqual(result.count, 1)
    }
    
    func testGetFiles() {
        let getAllFiles = cacheMa?.getFiles(parentPath: "/")
        //get file result is [SDML.SDMLNXLFile, SDML.SDMLNXLFile...]
        print("get file: \(getAllFiles?.value)")
        guard let files = getAllFiles?.value else {
            print("getFiles: \(getAllFiles?.error)")
            XCTAssertNil(getAllFiles?.error)
            return
        }
        print("--- NXL file number is: \(files.count) ---")
        //getfiles() result is array, so use loop to print files info
        print("-----------------------------------")
        for file in files {
            let nxlfile = file as? SDMLNXLFileInterface
            print("file name: \(nxlfile?.getName())")
            print("file DUID: \(nxlfile?.getDUID())")
            print("file rights: \(nxlfile?.getFileRights())")
            print("be shared or not: \(nxlfile?.isShare())")
            print("user recipients: \(nxlfile?.getRecipients())")
            print("be uploaded or not: \(nxlfile?.isUploaded())")
            print("-----------------------------------")
        }
        
        //logout and login other user, then get file
//        session?.logout(completion: {result in})
//
//        let json = RMCAPITest.Constant.rmtest
//        let cookieResponse = RMCAPITest.Constant.rmtestcookie
//        let getCookie = RMCAPITest.getHTTPCookiesFromString(from: cookieResponse)
//        let userLogin = self.session?.setLoginResult(cookies: getCookie!, response: json)
//
//        let getjulie = cacheMa?.getFiles(parentPath: "/")
//        print("---\(getjulie?.value) ---")
    }
  
    func testDecryptFile() {
        //read all cases from DecryptFile.txt, one element is one case from this Array
        let caseFile = RMCAPITest.testReadTXTFile(casefile: "DecryptFile")
        if caseFile .isEmpty {
            XCTAssert(false)
            print("------no content in this case file------")
            return
        }
        for cases in caseFile{
            //get all elements from one case, one case is one Array, then one elements is a parameter for decryptFile function
            let caseNum = cases.components(separatedBy: ";").dropLast()
            if caseNum[1] == "yes"{
                let getfiles = cacheMa?.searchFile(containName: caseNum[2])
                let saveFolder = caseNum[3]
                let overwrite = Bool.init(caseNum[4])
                for file in getfiles!.value! {
                    let nxlfile = file as? SDMLNXLFileInterface
                    cacheMa?.decryptFile(file: nxlfile!, toFolder: saveFolder, isOverwrite: overwrite!){
                        (path) in
                        //result "path" is the decryped file path: /Users/mini/Desktop/apitest/decrypt/1-2018-05-25-11-02-00.xlsb
                        guard let decryptedFile = path.value else{
                            print("-----case: \(caseNum[0]) decrypt file failed: \(path.error)-----")
                            XCTAssertNil(path.error)
                            print("----------------------------------------------------------")
                            return
                        }
                        ////path is the full path of decrypted file, /Users/mini/Desktop/apitest/decrypt/json-2018-04-26-15-57-53.txt
                        print("decrypt file is: \(decryptedFile) -----")
                        let fileManager = FileManager.default
                        if fileManager.fileExists(atPath: decryptedFile){
                            print("---Can find the decrypted file---")
                            print("{{{case: \(caseNum[0])--- pass ---}}}")
                            print("----------------------------------------------------------")
                        }else{
                            XCTAssert(false)
                            print("FIND DECRYPTED FILE FAILED")
                            print("{{{case: \(caseNum[0])--- Decrypt file failed ---}}}")
                            print("----------------------------------------------------------")
                        }
                    }
                }
            }else{
                print("{{{{case: \(caseNum[0])--- no need run ---}}}}")
                print("----------------------------------------------------------")
            }
        }
    }
    
    func testDeleteFile() {
        let getAllFiles = cacheMa?.searchFile(containName: "json1-2018-06-27-11-25-05.txt")
        XCTAssertNotEqual(0, getAllFiles!.value?.count)
        for file in (getAllFiles?.value)! {
            print("delete nxlfile value!!!!!: \(getAllFiles?.value)")
            let nxlfile = file as? SDMLNXLFileInterface
            let result = cacheMa?.deleteFile(file: nxlfile!)

            //if delete file successfully, then the value of result is nil
            guard let checkresult = result?.value else{
                print(" delete file failed: \(result?.error)")
                XCTAssertNil(result?.error)
                return
            }
        }
        //after delete file, check there is no files with expect file name
        let checkFile = cacheMa?.searchFile(containName: "json1-2018-06-27-11-25-05.txt").value
        XCTAssertEqual(0, checkFile?.count)
    }
    
    
    func testReshareFile() {
        //read all cases from ReshareFile.txt, one element is one case from this Array
        let caseFile = RMCAPITest.testReadTXTFile(casefile: "ReshareFile")
        if caseFile .isEmpty {
            XCTAssert(false)
            print("------no content in this case file------")
            return
        }
        for cases in caseFile{
            let caseNum = cases.components(separatedBy: ";").dropLast()
            if caseNum[1] == "yes"{
                let searchfiles = cacheMa?.searchFile(containName: caseNum[2]).value
                //just set the new recipients, for now, there is no add user and remove user parameters
                let newRecipients = RMCAPITest.getRecipients(emailAddress: caseNum[3])
                if searchfiles?.count == 0{
                    XCTAssert(false)
                    print("--- no file find ---")
                    return
                }
                for file in searchfiles! {
                    let nxlfile = file as? SDMLNXLFileInterface
                    cacheMa?.reshare(file: nxlfile!, newRecipients: newRecipients){
                        (result) in
                        //get reshare result, if successfully, then nil, if fail, return error
                        guard let shareResult = result.value else{
                            XCTAssertNil(result.error)
                            print("case:\(caseNum[0])----reshare file failed with \(result.error)----")
                            print("----------------------------------------------------------")
                            return
                        }
                        print("{case: \(caseNum[0]) --- Reshare file finished --- }")
                    }
                    let getResharedFile = nxlfile
                    let fileRecipients = getResharedFile?.getRecipients()
                    if newRecipients == fileRecipients! {
                        print("{case: \(caseNum[0]) --- Reshare file successfully --- }")
                        print("----------------------------------------------------------")
                    }else{
                        XCTAssertEqual(newRecipients, fileRecipients!)
                    }
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

extension CacheTestTests {
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

