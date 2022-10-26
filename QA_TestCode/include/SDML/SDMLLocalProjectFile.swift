//
//  SDMLLocalProjectFile.swift
//  SDML
//
//  Created by Ivan (Youmin) Zhang on 2019/4/2.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLLocalProjectFileInterface {
    
}

public class SDMLLocalProjectFile: SDMLNXLFile {
    var     parentFileId: String?
    var        projectId: Int?
    var   parentTenantId: String?
    var     parentPathId: String?
    var  parentProjectId: Int?
    var parentTenantName: String?
    var  tokenGroupName:String?
    
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
}

extension SDMLLocalProjectFile: SDMLLocalProjectFileInterface {
    
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
    
}
