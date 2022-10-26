//
//  CommonUtil.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 24/01/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

struct CommonUtil {
    static func generateClientId() -> String {
        return UUID().uuidString.replacingOccurrences(of: "-", with: "")
    }
    
    static func getDeviceId() -> String {
        return Host.current().localizedName ?? ""
    }
    
    static func createDefaultTenant() -> OCTenant {
        return OCTenant()
    }
    
    static func printTenant(_ tenant: OCTenant) {
        // Parameters
        print("Tenant ID: \(tenant.getTenant())")
        print("Tenant Router: \(tenant.getRouterURL())")
        
        // Query
        let urlRequest = tenant.getQuery()
        print("Tenant Query URL Request: \(urlRequest)")
        
        // Result
        print("Tenant RMS URL: \(tenant.getRMSURL())")
        print("Export to string: \(tenant.exportRMString())")
    }
    
    static func printURLRequest(_ request: URLRequest) {
    }
    
    static func printUser(_ user: OCUser) {
        print("User name: \(user.getName())")
        print("User email: \(user.getEmail())")
        print("User idpType: \(user.getIdpType())")
        print("User membership: \(user.getDefaultMembership())")
    }
}
