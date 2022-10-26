//
//  SDMLSessionInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 06/02/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation


public protocol SDMLSessionInterface {
    
    /// Get login manager.
    /// - returns: Login manager.
    func getLoginManager() -> SDMLResult<SDMLLoginManagerInterface>
    
    /// Get tenant instance from cache. Call 'fetchRMS' to create tenant if value is nil.
    /// - returns: Tenant instance.
    func getCurrentTenant() -> SDMLResult<SDMLTenantInterface>
    
    /// Get user instance from cache. Call 'setLoginResult' to create user if value is nil.
    /// - returns: User instance.
    func getCurrentUser() -> SDMLResult<SDMLUserInterface>
    
    /// Get heartbeat.
    /// - returns: Heartbeat instance.
    func getHeartbeat() -> SDMLUserPreferenceInterface?
    
    /// Get protect manager.
    /// - returns: Protect manager.
    func getProtectManager() -> SDMLResult<SDMLProtectManagerInterface>
    
    /// Get cache manager.
    /// - returns: Cache Manager instance.
    func getCacheManager() -> SDMLResult<SDMLCacheManagerInterface>
    
    /// Get upload manager.
    /// - returns: Upload manager instance.
    func getUploadManager() -> SDMLResult<SDMLSyncManagerInterface>
    
    /// Get downloadFileManager
    func getDownloadManager() -> SDMLResult<SDMLSyncManagerInterface>
    
    /// Get projectManager
    func getProjectManager() -> SDMLResult<SDMLProjectManagerInterface>
    
    /// Get myValut manager
    /// - Returns: myVault instance.
    func getMyVaultManager() -> SDMLResult<SDMLMyVaultManagerInterface>
    
    /// Get sharedWithMe manager
    ///
    /// - Returns: sharedWithMe instance
    func getShareWithMeManager() -> SDMLResult<SDMLSharedWithMeManagerInterface>
    
    /// Get system tag policy manager
    ///
    /// - Returns: systemTagPllicy instance
    func getSystemTagPolicyManager() -> SDMLResult<SDMLSystemTagPolicyManagerInterface>
    
    /// Get log manager.
    /// - Returns: Log manager instance.
    func getLogManager() -> SDMLResult<SDMLLogManagerInterface>
    
    /// Get service for not cache file.
    /// - Returns: File service.
    func getNXLFileManager() -> SDMLResult<SDMLNXLFileManagerInterface>
    
    /// Get remote view manager.
    func getRemoteViewManager() -> SDMLResult<SDMLRemoteViewManagerInterface>
    
    /// Get convert file manager.
    func getConvertFileManager() -> SDMLResult<SDMLConvertFileManagerInterface>
}
