//
//  OperationViewController.swift
//  RMCAPITest
//
//  Created by Paul (Qian) Chen on 19/04/2018.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import Cocoa

class OperationViewController: NSViewController {

    var test: RMCAPITest?
    
    var sessionId: String?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
//        guard let sessionId = sessionId else {
//            return
//        }
//
//        let test = RMCAPITest(identifier: sessionId)
//        test.getCurrentUser { (_) in
//        }
//
//        self.test = test
    }
    
    @IBAction func getTenant(_ sender: Any) {
        test?.testTenant()
    }
    
    @IBAction func getUser(_ sender: Any) {
        test?.testUser()
    }
    
    @IBAction func logout(_ sender: Any) {
        test?.testLogout()
    }
    
    @IBAction func protect(_ sender: Any) {
        test?.testProtectFile()
    }
    
    @IBAction func share(_ sender: Any) {
        test?.testShareFile()
    }
    
    @IBAction func getFiles(_ sender: Any) {
        test?.testGetFile() { files in
        }
    }
    @IBAction func searchFiles(_ sender: Any) {
        test?.testSearchFile() { files in
        }
    }
    
    @IBAction func clean(_ sender: Any) {
        test?.testCleanAllFile()
    }
    
    @IBAction func decryptFile(_ sender: Any) {
        test?.testDecryptFile()
    }
    
    @IBAction func reshareFile(_ sender: Any) {
        test?.testReshareFile()
    }
    
    @IBAction func testUploadFile(_ sender: Any) {
        test?.testUploadFile()
    }
    
    @IBAction func testCancelUploadFile(_ sender: Any) {
        test?.testCancelUploadFile()
    }
    
    @IBAction func getLogs(_ sender: Any) {
        test?.testGetLogs()
    }
    
    @IBAction func addLog(_ sender: Any) {
        test?.testAddLog()
    }
    
    @IBAction func getUsage(_ sender: Any) {
        test?.testGetUsage()
    }
    
    @IBAction func getHeartbeat(_ sender: Any) {
        test?.testHeartbeat()
    }
    
    @IBAction func decyptUnowned(_ sender: Any) {
        test?.testDecryptUnownedFile()
    }
    
    @IBAction func checkRight(_ sender: Any) {
        test?.testCheckRight()
    }
    @IBAction func remoteViewCache(_ sender: Any) {
        test?.testRemoteView()
    }
    
    @IBAction func convertFileCache(_ sender: Any) {
        test?.testConvertView()
    }
    
}
