//
//  SDMLNXLFile.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 14/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLNXLFileInterface: SDMLBaseFileInterface {
    
    /// Get duid.
    func getDUID() -> String
    
    /// Get recipients.
    func getRecipients() -> [String]
    
    /// Is upload.
    func isUploaded() -> Bool
    
    /// get the upload moudle
    ///
    /// - Returns: the upload moudle
    func getUploadTo() -> String
    
    /// Is share.
    func isShare() -> Bool
    
    /// Get right and obligation.
    func getRightObligation() -> SDMLRightObligation?
    
    /// Get source path.
    func getSourcePath() -> String
    
    /// Get comment.
    func getComment() -> String
    
    /// is Tag or ad-hoc.
    func isTagFile() -> Bool
    
    /// Get tag.
    func getTag() -> [String: [String]]
    /// Get whether the file was created by yourself
    ///
    /// - Returns: bool
    func getIsOwner() -> Bool
    
    /// getRightObligation of online
    ///
    /// - Parameter completion:
    func getOnlineRightObligation(completion:@escaping (SDMLResult<SDMLRightObligation?>) -> ())

}

public class SDMLNXLFile: SDMLBaseFile {
    var duid: String?
    var recipients: [String]?
    var uploaded: Bool?
    var uploadTo:String?
    var isLocation: Bool?
    var shared: Bool?
    var obRights: SDMLRightObligation?
    var sourcePath: String?
    var comment: String?
    var isTag: Bool?
    var tag: [String: [String]]?
    var isOwner: Bool?
    public func getOnlineRightObligation(completion: @escaping (SDMLResult<SDMLRightObligation?>) -> ()) {
        let ocNXLFile = CommonUtil.transform(from: self)
        self.obRights = CommonUtil.getAdhocRight(file: ocNXLFile)
        completion(.success(self.obRights))
    }
}

extension SDMLNXLFile: SDMLNXLFileInterface {
  
    
    public func getIsOwner() -> Bool {
      return  isOwner ?? false
    }
    
    public func getUploadTo() -> String {
        return uploadTo ?? ""
    }
    
    public func getDUID() -> String {
        return duid ?? ""
    }
    
    public func getRecipients() -> [String] {
        return recipients ?? []
    }
    
    public func isUploaded() -> Bool {
        return uploaded ?? false
    }
    
    public func isShare() -> Bool {
        return shared ?? false
    }
    
    public func getRightObligation() -> SDMLRightObligation? {
        return obRights
    }
    
    public func getSourcePath() -> String {
        return sourcePath ?? ""
    }
    
    public func getIsLocation() -> Bool {
        return isLocation ?? false
    }
    
    
    public func getComment() -> String {
        return comment ?? ""
    }
    
    public func isTagFile() -> Bool {
        return isTag ?? false
    }
    
    public func getTag() -> [String: [String]] {
        return tag ?? [:]
    }
}
