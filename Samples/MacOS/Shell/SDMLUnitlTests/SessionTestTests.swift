//
//  RMCAPITestTests.swift
//  RMCAPITestTests
//
//  Created by mini on 3/28/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML


class SessionTestTests: XCTestCase {
    
    private struct Constant{
        static let timeout: TimeInterval = 10
        static let userError = RMCAPITest.Constant.user404Response
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
    }
    
    func testRemveTenant() {
        let dic = UserDefaults.standard.dictionary(forKey: "Tenant")
        UserDefaults.standard.removeObject(forKey: "Tenant")
    }
    
    override func tearDown() {
        super.tearDown()
        //logout user after finish case
        //        self.session?.logout(completion: {result in
        //        })
    }
    
    func testSDML() {
        //get MACSDK version
        print("Library Version: \(getVersion())")
        //create an instance and find it exists
        let result = createInstance(identifier: "afd$100%nxmzvncx,zvkalfdsoafoafjdlsblxckbjvxkb/ndjafkzv20%zmvzvc")
        var instance = result.value
        print("instance created: \(instance)")
        XCTAssertNotNil(instance)
        //delete the instance and check there is no instance here
        deleteInstance(session: &instance)
        print("instance deleted: \(instance)")
        XCTAssertNil(instance)
    }
    
    func testFetchRMS(){
        //set different router and tenant, run fetchRMS to get RMS login URL
        //then check the result of router/tenant/rmsurl is the same with expect
        //read all cases from SessionTestTests.txt, one element is one case from this Array
        var caseFile = RMCAPITest.testReadTXTFile(casefile: "SessionTestTests")
        if caseFile .isEmpty{
            print("no content in case file")
            XCTAssert(false)
            return
        }else{
            for cases in caseFile{
                //get all elements from one case, one case is one Array, then one elements is a parameter for fetchRMS function
                let caseNum = cases.components(separatedBy: ";").dropLast()
                var run = caseNum[1]
                let routers = caseNum[2]
                let tenants = caseNum[3]
                if run == "yes"{
                    let fetchRMSCompletion: (SDMLResult<Any?>) -> () = {
                        result in
                        let curTenant = self.session?.getCurrentTenant().value
                        print("router-----: \(curTenant?.getRouterURL())")
                        print("tenant----: \(curTenant?.getTenant())")
                        print("RMSURL----: \(curTenant?.getRMSURL())")
                        XCTAssertEqual(caseNum[2], curTenant!.getRouterURL())
                        XCTAssertEqual(caseNum[3], curTenant?.getTenant())
                        XCTAssertEqual(caseNum[4], curTenant?.getRMSURL())
                        print("{{{{case: \(caseNum[0]),, pass}}}}")
                        print("----------------------------------------------------------")
                    }
                    initTenant(router: routers, tenant: tenants, completion: fetchRMSCompletion)
                }else{
                    print("{{{{case: \(caseNum[0]),, no need run}}}}")
                    print("----------------------------------------------------------")
                }
            }
        }
    }
    
    func testSetLoginResult() {
        let userJson = Constant.serverResponse
        let userCookieResponse = Constant.httpCookie
        let getCookie = RMCAPITest.getHTTPCookiesFromString(from: userCookieResponse)
        let result = self.session!.setLoginResult(cookies: getCookie!, response: userJson)
        print("~~~~ login user info: \(result.value) ~~~~~")
        //if setLoginResult is successful, then result.value should be nil, if no error, then login successfully
        XCTAssertNil(result.error)
    }
    
    func testGetLoginURLRequest() {
        let url = self.session?.getLoginURLRequest()
        print("URL is: \(url?.value)")
        print(url!.value!.urlRequest)
        XCTAssertNil(url?.error)
    }
    
    func test404Error() {
        let json = Constant.userError
        let cookieResponse = Constant.httpCookie
        let getCookie = RMCAPITest.getHTTPCookiesFromString(from: cookieResponse)
        let responseError = self.session?.setLoginResult(cookies: getCookie!, response: json).error
        print("login failed as ->>>>: \(responseError)")
        XCTAssertEqual("The account does not exist.", responseError as! String)
    }
    
    func testSuccessUser() {
        let user = self.session?.getCurrentUser().value
        print("user is: \(user!.getName())")
        print("email is: \(user!.getEmail())")
        print("idptype is: \(user!.getUserIdpType())")
        print("user id is: \(user!.getUserID())")
        XCTAssertEqual("Julie wawa", user?.getName())
        XCTAssertEqual("julie.fang@cn.nextlabs.com", user?.getEmail())
        XCTAssertEqual(SDML.UserIdpType.skydrm, user?.getUserIdpType())
        XCTAssertEqual("36", user?.getUserID())
    }
    
    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }
}

extension SessionTestTests {
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


