//
//  SDLError.swift
//  SDML
//
//  Created by Walt (Shuiping) Li on 13/12/2017.
//  Copyright © 2017 NextLabs. All rights reserved.
//

import Foundation

public enum SDMLError: Error {
    
    public enum FetchRMSFailureReason {
        case cannotChangeTenantAfterLogin
        case invalidRouterTenant
    }
    public enum ActivateTokenFailureReson {
        case cannotconnectedInternet
        case alreadyactivated
        case activatedTokenFailed
    }

    public enum LoginFailureReason {
        case cannotGetLoginURL
        case missingRequiredParameters
        case incorrectUsernamePassword
        case accountNotExist
        
        case noTenant
        case loadDBFailed(error: Error?)
    }
    
    public enum protectFailureReason {
        case getCertificateFailed
        case getTokenFailed
        case storeDBFailed
        case encyptFailed
        case alreadyNXLFile
        case noToken
        case activateTokenFailed
        
    }
    
    public enum UploadFailureReason {
        case getFileContentFailed(error: Error?)
        case serverFailed(error: Error?)
        case alreadyExist
        case storageExceeded
        case updateFailed
    }
    public enum DownloadFailureReason {
        case serverFailer(error:Error?)
        case haveNoRight
        case otherError(error:Error?)
        case connection
        case noToken
        case downloadFailde
    }
    
    public enum CacheFailureReason {
        case invaildParentPath
        case searchFileFailed
        case deleteFileFailed(error: Error?)
        case getDecyptTokenFailed
        case decyptFailed(error: Error?)
        case updateRecipientFailed(error: Error?)
    }
    
    public enum logFailureReason {
        case noLocalList
        case saveLogFailed
        case unsupportLogOperation
    }
    
    public enum myVaultFailureReason {
        case getUsageFailed
        case searchFileFailed
        case deleteFileFailed(error:Error?)
        case modelTransformFailed
        case downloadFilesFailed
        case getFileFailed
        case updateFileFailed
        case isNoDownload
    }
    public enum ProjectFailedReason {
        case getProjectListFailed
        case searchFileFailed
        case deleteFileFailed(error:Error?)
        case downloadFileFailed
        case modelTransformFailed
        case getProjectFolderFailed
        case getProjectFailed
        case getProjectFileFailed
        case updateProjectFileList
        case getProjectFileHeafInfoFailed
        case getProjectHeadInfoFailed
        case noSubProjectFiles
        case getProjectTagsFailed
    }
    public  enum TokenFailedReason {
        case lackOfToken
        case noneToken
        case getTTokenFailed
        case activetedTokenFailed
        case updateTokenStatusFaied
    }
    
    public enum shareWithMeFailureReason {
        case getUsageFailed
        case searchFileFailed
        case deleteFileFailed(error:Error?)
        case modelTransformFailed
        case downloadFilesFailed
        case getFileFailed
        case updateFileFailed
        case isNoDownload
    }
    
    
    public enum CommonFailedReason {
        case networkUnreachable
        case fileNotFound
        case notLogin
        case createFolderFailed(error: Error?)
        case invalidEmail
        case hasExpired
        case jsonSerializeFailed
        case missingURL
        case decryptFileFailed
        case denyPermission
    }
    
    public enum GetRightsObligationFailedReason {
        case noSession
        case getHeadInfo
        case downloadHeadInfo
        case heartInfoFailde
        case evaluationFailure
    }
    
    
    case undefine
    case fetchRMSFailed(reason: FetchRMSFailureReason)
    case activateTokenFailed(reason:ActivateTokenFailureReson)
    case loginFailed(reason: LoginFailureReason)
    case protectFailed(reason: protectFailureReason)
    case uploadFailed(reason: UploadFailureReason)
    case downloadFailed(reason:DownloadFailureReason)
    case cacheFailed(reason: CacheFailureReason)
    case activityLogFailed(reason: logFailureReason)
    case logoutFailed
    case myVaultFailed(reason: myVaultFailureReason)
    case commonFailed(reason: CommonFailedReason)
    case shareWithMeFailed(reason: shareWithMeFailureReason)
    case projectFailed(reason: ProjectFailedReason)
    case tokenFailed(reaseon:TokenFailedReason)
    case getRightsObligation(reason: GetRightsObligationFailedReason)
}

// MARK: - Error Booleans.

extension SDMLError {
    
    public var isUndefineError: Bool {
        if case .undefine = self { return true }
        return false
    }
    
    public var isFetchRMSError: Bool {
        if case .fetchRMSFailed = self { return true }
        return false
    }
    
    public var isLoginError: Bool {
        if case .loginFailed = self { return true }
        return false
    }
    
    public var isProtectError: Bool {
        if case .protectFailed = self { return true }
        return false
    }
    
    public var isUploadError: Bool {
        if case .uploadFailed = self { return true }
        return false
    }
    
    public var isCacheError: Bool {
        if case .cacheFailed = self { return true }
        return false
    }
    
    public var isActivityLogError: Bool {
        if case .activityLogFailed = self { return true }
        return false
    }
    
    public var isLogoutError: Bool {
        if case .logoutFailed = self { return true }
        return false
    }
    
    public var isMyVaultError: Bool {
        if case .myVaultFailed = self { return true }
        return false
    }
    
    public var isCommonError: Bool {
        if case .commonFailed = self { return true }
        return false
    }
}

extension SDMLError {
//    var underlyingError:
}
