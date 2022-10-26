//
//  UserViewController.swift
//  RMCAPITest
//
//  Created by Paul (Qian) Chen on 18/04/2018.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import Cocoa

struct LoginUser {
    var sessionId: String
    var userName: String
}

class UserViewController: NSViewController {

    @IBOutlet weak var tableView: NSTableView!
    
    var users = [LoginUser]()
    var tests = [RMCAPITest]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        tableView.delegate = self
        tableView.dataSource = self
        
        checkUserStatus()
    }
    
    func checkUserStatus() {
        for sessionId in SessionIDManager.shared.loadSessionID() {
            let test = RMCAPITest(identifier: sessionId)
            test.getCurrentUser() { user in
                if let user = user {
                    
                    self.tests.append(test)
                    
                    let user = LoginUser(sessionId: sessionId, userName: user.getName())
                    self.users.append(user)
                    self.tableView.reloadData()
                } else {
                    _ = SessionIDManager.shared.removeSessionID(sessionId: sessionId)
                }
            }
        }
    }
    
    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        if let webViewController = segue.destinationController as? WebViewController {
            
            webViewController.delegate = self
            
            if segue.identifier!.rawValue == "Login" {
                webViewController.type = .login
            } else if segue.identifier!.rawValue == "Register" {
                webViewController.type = .register
            }
            
        }
        
    }
}

extension UserViewController: WebViewControllerDelegate {
    func loginFinish(viewController: WebViewController) {
        // Login success.
        if let user = viewController.test?.getCurrentUser() {
            let sessionId = viewController.sessionId!
            SessionIDManager.shared.saveSessionID(sessionId: sessionId)
            
            self.tests.append(viewController.test!)
            
            let user = LoginUser(sessionId: sessionId, userName: user.getName())
            self.users.append(user)
            self.tableView.reloadData()
        }
        
        self.dismissViewController(viewController)
    }
}

extension UserViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        return users.count
    }
}

extension UserViewController: NSTableViewDelegate {
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        guard
            let identifier = tableColumn?.identifier,
            let cellView = tableView.makeView(withIdentifier: identifier, owner: self) as? CustomTableCellView
            else { return nil }
        
        let user = users[row]
        cellView.textField?.stringValue = user.userName
        cellView.subheadTextField.stringValue = user.sessionId
        
        cellView.test = self.tests[row]
        
        return cellView
        
    }
}
