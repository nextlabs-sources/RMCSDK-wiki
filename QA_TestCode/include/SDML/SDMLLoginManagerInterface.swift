//
//  SDMLLoginManagerInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 02/08/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLLoginManagerInterface {
    
    /// Init tenant.
    /// - Paramter router: Router.
    /// - Parameter tenantId: TenantId.
    func initTenant(router: String, tenantId: String)
    
    /// Check is user login.
    /// - returns: True if login.
    func isLogin() -> Bool
    
    /// Get login url.
    /// - Parameter completion: Async return cookie and login url request.
    func getLoginURLRequest(completion: @escaping (SDMLResult<(String, URLRequest)>) -> ())
    
    /// Get register url.
    /// - Parameter completion: Async return cookie and register url request.
    func getRegisterURLRequest(completion: @escaping (SDMLResult<(cookie: String, urlRequest: URLRequest)>) -> ())
    
    /// Set login result to login in client.
    /// - Paramter cookies: Cookies of login result.
    /// - Parameter response: Response of login result.
    /// - Parameter completion: Result of login in client.
    func setLoginResult(cookies: [HTTPCookie], response: String, completion: @escaping (SDMLResult<Any?>) -> ())
    
    /// Logout user.
    /// - Parameter completion: Logout result.
    func logout(completion: @escaping (SDMLResult<Any?>) -> ())
    
    
    
}
