//
//  SessionManager.swift
//  RMCAPITest
//
//  Created by Paul (Qian) Chen on 18/04/2018.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import Foundation
import SDML

class SessionIDManager {
    
    private struct Constant {
        static let kSessionID = "sessionId"
    }
    
    // Singleton
    static let shared = SessionIDManager()
    private init() {
    }
    
    func createSessionID() -> String {
        let sessionId = SessionIDManager.generateUUID()
        return sessionId
    }
    
    func saveSessionID(sessionId: String) -> Bool {
        var sessionIds = self.loadSessionID()
        sessionIds.append(sessionId)
        
        UserDefaults.standard.set(sessionIds, forKey: Constant.kSessionID)
        
        return true
    }
    
    func loadSessionID() -> [String] {
        if let sessionIds = UserDefaults.standard.object(forKey: Constant.kSessionID) as? [String] {
            return sessionIds
        }
        
        return []
    }
    
    func removeSessionID(sessionId: String) -> Bool {
        
        var result = false
        var sessionIds = self.loadSessionID()
        for (index, oldSessionId) in sessionIds.enumerated() {
            if oldSessionId == sessionId {
                sessionIds.remove(at: index)
                result = true
                break
            }
        }
        if result {
            UserDefaults.standard.set(sessionIds, forKey: Constant.kSessionID)
        }
        
        return result
    }
}

extension SessionIDManager {
    static func generateUUID() -> String {
        let uuid = UUID().uuidString
        return uuid
    }
}
