//
//  SDMLProjectUerInfo.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/1/29.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation
 public protocol SDMLUserInfoInterface {
    
    /// get the userID
    ///
    /// - Returns: the userID
    func getUserId() -> Int
    
    /// get the displayName
    ///
    /// - Returns: the displayName
    func getDisplayName() -> String
    
    /// the User`s email
    ///
    /// - Returns: the user`s email
    func getEmail() -> String
    
    /// get the CreationTime
    ///
    /// - Returns: the creationTime
    func getCreationTime() -> Date
}
public class SDMLUserInfo {
    var userId:Int?
    var displayName:String?
    var email:String?
    var creationTime:Date?
}
extension SDMLUserInfo:SDMLUserInfoInterface {
    public func getUserId() -> Int {
        return userId ?? 0
    }
    
    public func getDisplayName() -> String {
        return displayName ?? ""
    }
    
    public func getEmail() -> String {
        return email ?? ""
    }
    
    public func getCreationTime() -> Date {
        return creationTime ?? Date()
    }
    
    
}
