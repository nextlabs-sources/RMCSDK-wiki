//
//  SDMLMyvault.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2018/11/23.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLMyVaultInterface:SDMLNXLFileInterface {
    /// get FileLink
    ///
    /// - Returns: File link
    func getFileLink() -> String
    
    /// get the time of Shareon
    ///
    /// - Returns: share time
    func getShareOn() -> Int64
    
    /// get PathDisplay
    ///
    /// - Returns: displayPath
    func getPathDisplay() -> String
    
    /// get is Revoked
    ///
    /// - Returns: the satus of revoked
    func getIsRevoked() -> Bool
    
    
    /// get RepoId
    ///
    /// - Returns: repoid
    func getRepoId() -> Int64
    
    
    /// getCustomMetadata
    ///
    /// - Returns: data
    func getCustomMetadata() -> Data
    
    
    /// getIsDeleted
    ///
    /// - Returns: true or false
    func getIsDeleted() -> Bool
}

public class SDMLMyVaultFile: SDMLNXLFile {
     var            fileLink: String?
     var           isRevoked: Bool?
     var         pathDisplay: String?
     var              repoId: Int64?
     var             shareOn: Int64?
     var     customMetadata : Data?
     var             deleted: Bool?
     var  myVaultFileManager:SDMLMyVaultManager?
    public override func getOnlineRightObligation(completion: @escaping (SDMLResult<SDMLRightObligation?>) -> ()) {
        myVaultFileManager?.session?.nxlFileManager.getOnlineFileRightObligation(file: self, completion: { (rightObligation) in
            guard let right = rightObligation.value else {
                completion(.failure(SDMLError.getRightsObligation(reason: .noSession)))
                return
            }
            self.obRights = right
            completion(.success(right))
        })
    }
}

extension SDMLMyVaultFile : SDMLMyVaultInterface {
    
    public func getRepoId() -> Int64 {
        return repoId ?? 0
    }
    
    public func getIsRevoked() -> Bool {
        return isRevoked ?? false
    }
    
    public func getPathDisplay() -> String {
        return pathDisplay ?? ""
    }
    
    public func getShareOn()-> Int64 {
        return shareOn ?? 0
    }
    
    public func getFileLink() -> String {
        return fileLink ?? ""
    }
   
    
    public func getCustomMetadata() -> Data {
        return customMetadata ??  Data()
    }
    
    public func getIsDeleted() -> Bool {
        return deleted ?? false
    }
}
