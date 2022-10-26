//
//  RMCAPITest.swift
//  RMCAPITest
//
//  Created by Paul (Qian) Chen on 15/01/2018.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import Foundation
import SDML


class RMCAPITest {
    
    struct Constant {
        static let router = //"https://rmtest.nextlabs.solutions"
        "https://rms-centos7303.qapf1.qalab01.nextlabs.com:8443"
        static let tenant = ""
        //"skydrm.com"
        //"ee4eddd8-81fc-4488-a1a7-2a71505c44b2"
    }
    
    init(identifier: String) {
        let result = createInstance(identifier: identifier)
        session = result.value!
    }
    
    var session: SDMLSessionInterface
}

// MARK: Before login.

// MARK: SDK API methods.
extension RMCAPITest {
    static func testSDMLAPI() {
        print("Library Version: \(getVersion())")
        let result = createInstance(identifier: "a")
        var instance = result.value
        print("instance created: \(instance)")
        deleteInstance(session: &instance)
        print("instance deleted: \(instance)")
    }
}

extension RMCAPITest {
    
    func getLoginManager() -> SDMLLoginManagerInterface {
        return (session.getLoginManager().value)!
    }
    
    func testLogin(completion: @escaping (String, URLRequest) -> Void) {
        let manager = getLoginManager()
        manager.initTenant(router: Constant.router, tenantId: Constant.tenant)
        manager.getLoginURLRequest() { result in
            guard let value = result.value else {
                print(result.error)
                return
            }
            
            completion(value.0, value.1)
        }
        
    }
    
    
    func testRegister(completion: @escaping (String, URLRequest) -> Void) {
        let manager = getLoginManager()
        manager.getRegisterURLRequest() { result in
            guard let value = result.value else {
                print(result.error)
                return
            }
            
            completion(value.0, value.1)
        }
        
    }
    
    func testLoginWithResponse(cookies: [HTTPCookie], str: String, completion: @escaping () -> ()) {
        
        // It's better.
//        let data = NSKeyedArchiver.archivedData(withRootObject: cookies)
//        let cookiesBack = NSKeyedUnarchiver.unarchiveObject(with: data)
//        if let cookies = cookiesBack as? [HTTPCookie] {
//            print(cookies)
//        }
        
        // Test cookie and string for unit test use.
//        let cookiesStr = RMCAPITest.getStringFromCookies(from: cookies)
//        let cookiesBack = RMCAPITest.getHTTPCookiesFromString(from: cookiesStr!)
        
        let manager = getLoginManager()
        manager.setLoginResult(cookies: cookies, response: str) { (result) in
            if let error = result.error {
                print("setLoginResult: \(error)")
            } else {
                print("login success")
            }
            
            completion()
        }
        
    }
}

// MARK: After login.

extension RMCAPITest {
    func getCurrentUser() -> SDMLUserInterface? {
        return self.session.getCurrentUser().value
    }
    
    func getCurrentUser(router: String = "", tenant: String = "", completion: @escaping (SDMLUserInterface?) -> ()) {
        // Get RMS server.
//        session.initTenant(router: Constant.router, tenant: Constant.tenant) { (_) in
//            // Has login or not.
//            if let user = self.session.getCurrentUser().value {
//                completion(user)
//            } else {
//                completion(nil)
//            }
//        }
        
        let manager = getLoginManager()
        manager.initTenant(router: Constant.router, tenantId: Constant.tenant)
        if manager.isLogin() {
            completion((session.getCurrentUser().value)!)
        } else {
            completion(nil)
        }
//        if let user = self.session.getCurrentUser().value {
//            completion(user)
//        } else {
//            completion(nil)
//        }
    }
}

// MARK: Operations.

extension RMCAPITest {
    
    // MARK: Tenant.
    
    func testTenant() {
        let result = self.session.getCurrentTenant()
        guard let tenant = result.value else {
            print("getCurrentTenant: \(result.error)")
            return
        }
        print("router: \(tenant.getRouterURL())")
        print("tenant: \(tenant.getTenant())")
        print("RMSURL: \(tenant.getRMSURL())")
    }
    
    // MARK: User.
    
    func testUser() {
        let result = self.session.getCurrentUser()
        guard let user = result.value else {
            print("getCurrentUser: \(result.error)")
            return
        }
        
        print("user name: \(user.getName())")
        print("user email: \(user.getEmail())")
        print("user idpType: \(user.getUserIdpType())")
        print("user id: \(user.getUserID())")
    }
    
    // MARK: Logout.
    
    func testLogout() {
        let manager = getLoginManager()
        manager.logout { (result) in
            if let error = result.error {
                print("logout: \(error)")
                return
            }
            
            print("logout: success")
        }
    }
    
    // MARK: Protect.
    
    func getProtectManager() -> SDMLProtectManagerInterface? {
        let result = self.session.getProtectManager()
        guard let manager = result.value else {
            print("getProtectManager: \(result.error)")
            return nil
        }
        
        return manager
    }
    
    func openPanel(completion: @escaping (URL) -> ()) {
        let openPanel = NSOpenPanel()
        openPanel.canChooseFiles = true
        openPanel.canChooseDirectories = false
        openPanel.worksWhenModal = true
        openPanel.allowsMultipleSelection = false
        
        openPanel.begin() { (response) in
            if response == NSApplication.ModalResponse.OK {
                completion(openPanel.url!)
            }
        }
    }
    
    func testProtectFile() {
        openPanel { (url) in
//            self.protectUsingRights(path: url.path)
            self.protectUsingTags(path: url.path)
        }
        
    }
    
    func protectUsingRights(path: String) {
        guard let manager = getProtectManager() else {
            return
        }
        
        let rights1 = [SDMLFileRight.view, SDMLFileRight.print, SDMLFileRight.share]
        let watermark = SDMLWatermark(text: "aaa")
        
        let start = UInt64(Date().timeIntervalSince1970)
        let end = UInt64(Date(timeIntervalSinceNow: 10000).timeIntervalSince1970)
        let expiry = SDMLExpiry(type: .relativeExpire, start: start, end: end)
        //        let expiry = SDMLExpiry(type: .neverExpire, start: 0, end: 0)
        let obRights = SDMLRightObligation(rights: rights1, watermark: watermark, expiry: expiry)
        
        manager.protectOriginFile(path: path, rights: obRights) { (result) in
            guard let file = result.value else {
                print("protectOriginFile: \(result.error)")
                return
            }
            
            self.testNXLFile(file: file)
        }
    }
    
    func protectUsingTags(path: String) {
        guard let manager = getProtectManager() else {
            return
        }
        
        let tags = ["name": ["paul", "qian"], "numbers": ["1", "2"]]
        manager.protectOriginFile(path: path, tags: tags) { (result) in
            guard let file = result.value else {
                print("protectOriginFile: \(result.error)")
                return
            }
            
            self.testNXLFile(file: file)
        }
    }
    
    func testShareFile() {
//        shareUsingRights()
        shareUsingTag()
    }
    
    func shareUsingTag() {
        guard let manager = getProtectManager() else {
            return
        }
        
        let path1 = "/Users/pchen/Temp/aaa.txt"
        let tags = ["name": ["paul", "qian"], "numbers": ["1", "2"]]
        let recipients = ["paul.chen@nextlabs.com"]
        let comment = "abcdefg"
        manager.shareOriginFile(path: path1, tags: tags, recipients: recipients, comment: comment) { (result) in
            guard let file = result.value else {
                print("protectOriginFile: \(result.error)")
                return
            }
            
            self.testNXLFile(file: file)
        }
    }
    
    func shareUsingRights() {
        guard let manager = getProtectManager() else {
            return
        }
        
        let path1 = "/Users/pchen/Temp/aaa.txt"
        let rights1 = [SDMLFileRight.view]
        let recipients = ["paul.chen@nextlabs.com"]
        let comment = "abcdefg"
        let watermark = SDMLWatermark(text: "")
        
//        let start = UInt64(Date().timeIntervalSince1970)
//        let end = UInt64(Date(timeIntervalSinceNow: 10000).timeIntervalSince1970)
//        let expiry = SDMLExpiry(type: .relativeExpire, start: start, end: end)
        
        let obRights = SDMLRightObligation(rights: rights1, watermark: watermark, expiry: nil)
        manager.shareOriginFile(path: path1, rights: obRights, recipients: recipients, comment: comment) { (result) in
            guard let file = result.value else {
                print("protectOriginFile: \(result.error)")
                return
            }
            
            self.testNXLFile(file: file)
        }
    }
    
    // MARK: Cache.
    
   func getCacheManager() -> SDMLCacheManagerInterface? {
        let result = self.session.getCacheManager()
        guard let manager = result.value else {
            print("getCacheManager: \(result.error)")
            return nil
        }
        
        return manager
    }
    
    func testGetFile(completion: @escaping ([SDMLBaseFile]) -> ()) {
        guard let manager = getCacheManager() else {
            completion([])
            return
        }
        
        DispatchQueue.global().async {
            manager.getFiles(parentPath: "/") { result in
                guard let files = result.value else {
                    print("getFiles: \(result.error)")
                    return
                }
                
                for file in files {
                    if let nxlFile = file as? SDMLNXLFile {
                        self.testNXLFile(file: nxlFile)
                    } else {
                        self.testFile(file: file)
                    }
                }
                
                completion(files)
            }
        }
        
        
    }
    
    func testSearchFile(name: String = "aaa", compleiton: @escaping ([SDMLBaseFile]) -> ()) {
        guard let manager = getCacheManager() else {
            return
        }
        
        let result = manager.searchFile(containName: name) { result in
            guard let files = result.value else {
                print("getFiles: \(result.error)")
                return
            }
            
            for file in files {
                if let nxlFile = file as? SDMLNXLFile {
                    self.testNXLFile(file: nxlFile)
                } else {
                    self.testFile(file: file)
                }
            }
            
            compleiton(files)
        }
    }
    
    func testCleanAllFile() {
        testGetFile() { files in
            for file in files {
                self.testDeleteFile(file: file)
            }
        }
        
    }
    
    func testDecryptFile() {
        testGetFile() { files in
            for file in files {
                if let nxlFile = file as? SDMLNXLFile {
                    self.testDecyptFile(file: nxlFile)
                }
            }
        }
        
    }
    
    func testReshareFile() {
        testSearchFile() { files in
            for file in files {
                if let nxlFile = file as? SDMLNXLFile {
                    self.testReshareFile(file: nxlFile)
                }
            }
        }
        
    }
    
    // MARK: Upload file.
    
    func getUploadManager() -> SDMLSyncManagerInterface? {
        return session.getUploadManager().value
    }
    
    func testUploadFile() {
        testGetFile() { files in
            for file in files {
                if let nxlFile = file as? SDMLNXLFile {
                    //                nxlFile.isUploaded() == false {
                    self.testUploadFile(file: nxlFile)
                }
            }
        }
        
    }
    
    func testCancelUploadFile() {
        testSearchFile() { files in
            for file in files {
                if let nxlFile = file as? SDMLNXLFile,
                    nxlFile.isUploaded() == false {
                    self.testCancelUploadFile(file: nxlFile)
                }
            }
        }
        
    }
    
    // MARK: Myvault.
    
    private func getMyVault() -> SDMLMyVaultManagerInterface? {
        guard let myvault = self.session.getMyVaultManager().value else {
            return nil
        }
        
        return myvault
    }
    
    func testGetUsage() {
        guard let myvault = getMyVault() else {
            return
        }
        
        myvault.getStorage { (result) in
            if let usage = result.value?.usage, let quota = result.value?.quota {
                let usageString = ByteCountFormatter.string(fromByteCount: Int64(usage), countStyle: ByteCountFormatter.CountStyle.file)
                let quotaString = ByteCountFormatter.string(fromByteCount: Int64(quota), countStyle: ByteCountFormatter.CountStyle.file)
                print("usage: \(usageString), quota: \(quotaString)")
            }
        }
    }
    
    // MARK: Heartbeat.
    
    func testHeartbeat() {
        let heartbeat = session.getHeartbeat()
    }
    
    // MARK: NXL.
    
    func getNXLFileService() -> SDMLNXLFileManagerInterface? {
        return session.getNXLFileManager().value
    }
    
    func testDecryptUnownedFile() {
        guard let service = getNXLFileService() else {
            return
        }
        let path = "/Users/pchen/Temp/aaa-2018-09-05-16-18-37.txt.nxl"
        let toFolder = "/Users/pchen/Desktop/"
        service.decryptFile(path: path, toFolder: toFolder, isOverwrite: false) { (result) in
        }
    }
    
    func testGetMetadata() {
        guard let service = getNXLFileService() else {
            return
        }
        
        let path = "/Users/pchen/Desktop/a-2018-06-05-17-43-27.txt.nxl"
//        service.getMetadata(path: path) { (result) in
//            if let file = result.value {
//                self.testNXLFile(file: file)
//            }
//        }
    }
    
    func testCheckRight() {
        guard let service = getNXLFileService() else {
            return
        }
        let path = "/Users/pchen/Desktop/aaa-2018-08-30-17-10-47.txt.nxl"
        service.getRight(path: path) { (result) in
        }
    }
    
    func testReshareNXL() {
    }
    
    // MARK: Activity log.
    
    func getActivityLogManager() -> SDMLLogManagerInterface? {
        return session.getLogManager().value
    }
    
    func testGetLogs() {
        testGetFile() { files in
            for file in files {
                if let nxlFile = file as? SDMLNXLFile {
                    self.testGetLogs(file: nxlFile)
                }
            }
        }
        
    }
    
    func testAddLog() {
        testGetFile() { files in
            for file in files {
                if let nxlFile = file as? SDMLNXLFile {
                    self.testAddLog(file: nxlFile)
                }
            }
        }
        
        
    }
    
    // MARK:
    func testRemoteView() {
        let manager = self.session.getRemoteViewManager().value!
        let path = "/Users/pchen/Temp/README.md"
        manager.viewLocal(path: path, progress: nil) { (result) in
            print(result.value)
        }
    }
    
    func testConvertView() {
        let manager = self.session.getConvertFileManager().value!
        let path = "/Users/pchen/Desktop/race-car_x_t.jt"
        manager.convert(path: path, toFormat: "hsf", toFolder: "/Users/pchen/Temp/", isOverwrite: false, progress: nil) { (result) in
            print(result.value)
        }
    }
}

// MARK: Private methods.

extension RMCAPITest {
    
    private func testNXLFile(file: SDMLNXLFile) {
        print("NXL DUID: \(file.getDUID())")
        
        print("NXL Recipients: \(file.getRecipients())")
        print("NXL Uploaded: \(file.isUploaded())")
        print("NXL Source Path: \(file.getSourcePath())")
        let obRights = file.getRightObligation()
        print("NXL Rights: \(obRights?.rights)")
        print("NXL watermark: \(obRights?.watermark?.getText())")
        let expiry = obRights?.expiry
        print("NXL expiry: \(expiry?.type)\(expiry?.start)\(expiry?.end)")
        print("tag: \(file.getTag())")
        print("isTagFile: \(file.isTagFile())")
        
        testFile(file: file)
    }
    
    private func testFile(file: SDMLBaseFile) {
        print("File Name: \(file.getName())")
        print("File Kind: \(file.getKind())")
        print("File Path ID: \(file.getPathID())")
        print("File Size: \(file.getSize())")
        print("File Created: \(file.getCreatedTime())")
        print("File Modified: \(file.getModifiedTime())")
        print("File Local Path: \(file.getLocalPath())")
        
    }
    
    private func testLog(log: SDMLActivityLogInterface) {
        print("log file duid: \(log.getDUID())")
        print("log file name: \(log.getName())")
        print("log operation: \(log.getOperation())")
        print("log accessTime: \(log.getAccessTime())")
        print("log isAllowed: \(log.isAllowed())")
    }
    
    private func testDecyptFile(file: SDMLNXLFile) {
        guard let manager = getCacheManager() else {
            return
        }
        let toFolder = "/Users/pchen/Desktop/"
        manager.decryptFile(file: file, toFolder: toFolder, isOverwrite: true) {
            result in
            print(result.value)
        }
    }
    
    private func testDeleteFile(file: SDMLBaseFile) {
        guard let manager = getCacheManager() else {
            return
        }
        
        manager.deleteFile(file: file)
    }
    
    private func testReshareFile(file: SDMLNXLFile) {
        guard let manager = getCacheManager() else {
            return
        }
        let newRecipients = ["346285234@qq.com"]
        manager.reshare(file: file, newRecipients: newRecipients, comment: "") { (result) in
            print("reshare: \(result.error)")
        }
    }
    
    private func testUploadFile(file: SDMLNXLFile) {
        guard let manager = getUploadManager() else {
            return
        }
        
        let str = manager.upload(file: file, progress: { (_) in
        }) { (result) in
            self.testNXLFile(file: file)
        }
    }
    
    private func testCancelUploadFile(file: SDMLNXLFile) {
        guard let manager = getUploadManager() else {
            return
        }
        
        let str = manager.upload(file: file, progress: { (_) in
        }) { (result) in
        }
        manager.cancel(id: str)
    }
    
    private func testGetLogs(file: SDMLNXLFile) {
        guard let manager = getActivityLogManager() else {
            return
        }
        manager.getLogList(file: file) { (result) in
            if let logs = result.value {
                for log in logs {
                    self.testLog(log: log)
                }
            }
            
        }
    }
    
    private func testAddLog(file: SDMLNXLFile) {
        guard let manager = getActivityLogManager() else {
            return
        }
        
        let log = SDMLActivityLog(email: "", operation: .view, isAllow: true, accessTime: Date(), fileDuid: "", fileName: "")
        manager.addLog(file: file, log: log) { (result) in
        }
    }
}

// MARK: Transform cookies with Array

extension RMCAPITest {
    
    static func printCookie(cookies: [HTTPCookie]) {
        print(getStringFromCookies(from: cookies))
    }
    
    static func getStringFromCookies(from: [HTTPCookie]) -> String? {
        var to = [[String: String]]()
        transform(from: from, to: &to)
        do {
            let data = try JSONSerialization.data(withJSONObject: to, options: .prettyPrinted)
            let str = String(data: data, encoding: .utf8)
            return str
        } catch {
            print(error)
        }
        
        return nil
    }
    
    static func getHTTPCookiesFromString(from: String) -> [HTTPCookie]? {
        do {
            let data = from.data(using: .utf8)!
            let value = try JSONSerialization.jsonObject(with: data, options: .allowFragments)
            if let array = value as? [[String: String]] {
                var cookies = [HTTPCookie]()
                for dic in array {
                    var properties = [HTTPCookiePropertyKey: Any]()
                    transform(from: dic, to: &properties)
                    if let cookie = HTTPCookie(properties: properties) {
                        cookies.append(cookie)
                    }
                }
                return cookies
            }
        } catch {
            print(error)
        }
        
        return nil
    }
    

    static func transform(from: [HTTPCookie], to: inout [[String: String]]) {
        for cookie in from {
            if let properties = cookie.properties {
                var dic = [String: String]()
                transform(from: properties, to: &dic)
                to.append(dic)
            }
        }
    }
    
    // For Testing, remove not string value.
    static func transform(from: [HTTPCookiePropertyKey: Any], to: inout [String: String]) {
        for cookie in from {
            if let str = cookie.value as? String {
                to[cookie.key.rawValue] = str
            }
        }
    }
    
    static func transform(from: [String: String], to: inout [HTTPCookiePropertyKey: Any]) {
        for pair in from {
            to[HTTPCookiePropertyKey.init(pair.key)] = pair.value
        }
    }
    
    //    static func transform(from: String) -> [HTTPCookie]? {
    //        do {
    //            let data = from.data(using: .utf8)!
    //            let value = try JSONSerialization.jsonObject(with: data, options: .allowFragments)
    //            if let cookies = value as? [HTTPCookie] {
    //                return cookies
    //            }
    //        } catch {
    //            print(error)
    //        }
    //
    //        return nil
    //    }
    //
}
