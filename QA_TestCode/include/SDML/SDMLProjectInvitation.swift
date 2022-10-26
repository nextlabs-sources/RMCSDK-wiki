//
//  SDMLInvitation.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/2/21.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

protocol SDMLProjectInvitationInterface {

    /// get the invitation invitationId
    ///
    /// - Returns: the invitationId
    func getInvitationId() -> String
    
    /// get the inviteeEmail
    ///
    /// - Returns: the invitee email
    func getInviteeEmail() -> String
    
    /// get the inviterDisplayName
    ///
    /// - Returns: the inviterDisplatName
    func getInviterDisplayName() -> String
    
    /// get the inviterEmail
    ///
    /// - Returns: the inviterEmail
    func getInviterEmail() -> String
    
    /// get the inviterTime
    ///
    /// - Returns: the inviterTime
    func getInviteTime() -> TimeInterval
    
    
    
}

class SDMLProjectInvitation {
    var invitationId:String?
    var inviteeEmail:String?
    var inviterEmail:String?
    var inviterDisplayName:String?
    var inviteTime:TimeInterval?
    var project:SDMLProject?
}

extension SDMLProjectInvitation:SDMLProjectInvitationInterface {
    func getInviteTime() -> TimeInterval {
        return inviteTime ?? 0.0
    }
    
  
    
    func getInvitationId() -> String {
        return invitationId ?? ""
    }
  
    
    func getInviteeEmail() -> String {
        return inviteeEmail ?? ""
    }
    
    func getInviterDisplayName() -> String {
        return inviterDisplayName ?? ""
    }
    
    func getInviterEmail() -> String {
        return inviteeEmail ?? ""
    }
}
