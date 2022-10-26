//
//  SDMLUserInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 06/02/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLUserInterface {
    /// Get user name.
    func getName() -> String
    
    /// Get user id.
    func getUserID() -> String
    
    /// Get user email.
    func getEmail() -> String
    
    /// Get user idp type.
    func getUserIdpType() -> UserIdpType?
}
