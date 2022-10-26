//
//  SDMLOutBoxFile.swift
//  SDML
//
//  Created by Ivan (Youmin) Zhang on 2019/4/2.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLOutBoxFileInterface {
    
}
public enum outBoxFileType {
    case myVaultFile
    case projectFile
}
public class SDMLOutBoxFile: SDMLNXLFile {
    var     parentFileId: String?
    var        projectId: Int?
    var   parentTenantId: String?
    var     parentPathId: String?
    var  parentProjectId: Int?
    var parentTenantName: String?
    var   tokenGroupName:String?
    var      outFileType: outBoxFileType?
    var          session:SDMLSession?
    
    func getParentTenantName() -> String {
        return parentTenantName ?? ""
    }
    
    func getParentProjectId() -> Int {
        return parentProjectId ?? 0
    }
    
    func getParentTenantId() -> String {
        return parentTenantId ?? ""
    }
    
    func getParentPathId() -> String {
        return parentPathId ?? ""
    }
    func getTokenGroupName() -> String {
        return tokenGroupName ?? ""
    }
    
    public override func getOnlineRightObligation(completion: @escaping (SDMLResult<SDMLRightObligation?>) -> ()) {
      session?.nxlFileManager.getOnlineFileRightObligation(file: self, completion: { (rightObligation) in
            guard let right = rightObligation.value else {
                completion(.failure(SDMLError.getRightsObligation(reason: .noSession)))
                return
            }
            self.obRights = right
            completion(.success(right))
        })
    }
}

extension SDMLOutBoxFile: SDMLOutBoxFileInterface {
    
    /// Gets parent file folder id
    ///
    /// - Returns: id
    public func getParentFileId() -> String {
        return parentFileId ?? ""
    }
    
    /// Gets project id
    ///
    /// - Returns: id
    public func getProjectId() -> Int {
        return projectId ?? 0
    }
    public func getOutBoxFileType() -> outBoxFileType {
        return outFileType ?? .myVaultFile
    }
    
}
