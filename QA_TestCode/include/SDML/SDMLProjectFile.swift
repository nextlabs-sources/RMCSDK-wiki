//
//  SDMLProject.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/1/26.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLProjectFileInterface:SDMLNXLFileInterface {
    
    /// get the converted_path
    ///
    /// - Returns: return the converted_path
    func getConverted_path() -> String
    
    /// get the FileType
    ///
    /// - Returns: the FileType
    func getFileType() -> String
    
    /// judge the File weather is the folder
    ///
    /// - Returns: the judge resul
    func getIsFolder() -> Bool
    
    /// judge the file weather is the NXL file
    ///
    /// - Returns: the judege result
    func getIsNXL() -> Bool
    
    /// get the LastModifiedUserInfo
    ///
    /// - Returns: the lastModifiedUserInfo
    func getLastModifiedUser() -> SDMLUserInfo
    
    /// get the file owner
    ///
    /// - Returns: the file  owner info
    func getOwner() -> SDMLUserInfo?
    
    
    /// get the file id
    ///
    /// - Returns: the id
    func getFileId() -> String
    
    
    /// get project id
    ///
    /// - Returns: the project id
    func getProjectId() -> Int
    
    
    
    /// get bool , true mean is root
    ///
    /// - Returns:
    func getIsRoot() -> Bool
    
    
    /// gets the parent file of current file
    ///
    /// - Returns: projectFile
    func getParentFile() -> SDMLProjectFile?
    
    /// gets subfiles of the current file
    ///
    /// - Returns: projectFile
    func getSubFiles() -> [SDMLProjectFile]
    
    /// gets pathDisplay
    ///
    /// - Returns: pathDisplay
    func getPathDisplay() -> String
    
    //gets project of the projectFile
    func getProject() -> SDMLProject
    
    //judge the user wearther is file owner
    func isFileOwner() -> Bool
      /// get File right
      ///
      /// - Parameter completion: return file rights
    func getOnlineRightObligation(completion: @escaping (SDMLResult<SDMLRightObligation?>) -> ())
}

public class SDMLProjectFile: SDMLNXLFile {
    var converted_path: String?
    var fileType: String?
    var isFolder: Bool?
    var isNXL: Bool?
    var lastModifiedUser:SDMLUserInfo?
    var owner:SDMLUserInfo?
    var fileId: String?
    var isRoot: Bool?
    var projectId: Int?
    weak var parentFile: SDMLProjectFile?
    var subfiles: [SDMLProjectFile]?
    var pathDisplay: String?
    var project: SDMLProject?
    public  override func getOnlineRightObligation(completion: @escaping (SDMLResult<SDMLRightObligation?>) -> ()) {
        project?.session?.nxlFileManager.getOnlineFileRightObligation(file: self, completion: { (rightObligation) in
            guard let right = rightObligation.value else {
                completion(.failure(SDMLError.getRightsObligation(reason: .noSession)))
                return
            }
            self.obRights = right
            completion(.success(right))
        })
    }
}

extension SDMLProjectFile:SDMLProjectFileInterface {
 
    public func isFileOwner() -> Bool {
        return isOwner ?? false
    }
    
    public func getProject() -> SDMLProject {
        return project ?? SDMLProject()
    }
    
    public func getFileId() -> String {
        return fileId ?? ""

    }
    
    public func getPathDisplay() -> String {
        return pathDisplay ?? ""
    }
    
    
    public func getProjectId() -> Int {
        return projectId ?? 0
    }
    
    public func getIsRoot() -> Bool {
        return isRoot ?? false
    }
    
    public func getParentFile() -> SDMLProjectFile? {
        return parentFile ?? nil
    }
    
    public func getSubFiles() -> [SDMLProjectFile] {
        return subfiles ?? []
    }
    
    public func getIsNXL() -> Bool {
        return isNXL ?? false
    }

    public func getOwner() -> SDMLUserInfo? {
        return owner
    }
    
    public func getLastModifiedUser() -> SDMLUserInfo {
        return lastModifiedUser ?? SDMLUserInfo()
    }
    
    public func getConverted_path() -> String {
        return converted_path ?? ""
    }
    
    public func getFileType() -> String {
        return fileType ?? ""
    }
    
    public func getIsFolder() -> Bool {
        return isFolder ?? false
    }
    
}


