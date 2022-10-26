//
//  SDMLTenantInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 06/02/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLTenantInterface {
    /// Get tenant id.
    func getTenant() -> String
    
    /// Get router url.
    func getRouterURL() -> String
    
    /// Get rms address.
    func getRMSURL() -> String
}
