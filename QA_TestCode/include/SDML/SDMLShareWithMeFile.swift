
//
//  File.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2018/12/28.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLShareWithMeFileInterface:SDMLNXLFileInterface {
    
    /// getFileType
    ///
    /// - Returns:the file Type
    func getFileType() -> String
    
    /// get the ShareBy
    ///
    /// - Returns: the ShareBy
    func getSharedBy()-> String
    
    /// get the shareFileLink
    ///
    /// - Returns: the share file Link
    func getSharedLink() -> String
    
    /// get the file shareDate
    ///
    /// - Returns: the file shareDate
    func getSharedDate() ->Date
    
    /// get the transactionCode
    ///
    /// - Returns: the share files transactionCode
    func getTransactionCode() -> String
    
    /// get the transactionId
    ///
    /// - Returns: transactionId
    func getTransactionId() -> String
    
}
public class SDMLShareWithMeFile: SDMLNXLFile {
  var            fileType: String?
  var            sharedBy: String?
  var          sharedLink: String?
  var          sharedDate: Date?
  var     transactionCode: String?
  var       transactionId: String?
  var    sdmlShareManager: SDMLSharedWithMeManager?
    
public override func getOnlineRightObligation(completion: @escaping (SDMLResult<SDMLRightObligation?>) -> ()) {
        sdmlShareManager?.session?.nxlFileManager.getOnlineFileRightObligation(file: self, completion: { (rightObligation) in
            guard let right = rightObligation.value else {
                completion(.failure(SDMLError.getRightsObligation(reason: .noSession)))
                return
            }
            self.obRights = right
            completion(.success(right))
        })
    }
}
extension SDMLShareWithMeFile:SDMLShareWithMeFileInterface {
    
    public func getFileType() -> String {
        return  fileType ?? ""
    }
    
    public func getSharedBy() -> String {
        return sharedBy ?? ""
    }
    
    public func getSharedLink() -> String {
        return sharedLink ?? ""
    }
    
    public func getSharedDate() -> Date {
        return sharedDate ?? Date()
    }
    
    public func getTransactionCode() -> String {
        return transactionCode ?? ""
    }
    
    public func getTransactionId() -> String {
        return transactionId ?? ""
    }
    
    



}
