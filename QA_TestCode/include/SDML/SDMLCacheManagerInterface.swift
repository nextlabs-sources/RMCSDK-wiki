//
//  SDMLFileManagerInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 06/02/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

/// Manager file operations.
public protocol SDMLCacheManagerInterface {
    /// Get files under path.
    /// - Parameter parentPath: ParentPath, if root use "/".
    /// - returns: Basefile or error.
    func getFiles(parentPath: String, completion: @escaping (SDMLResult<[SDMLOutBoxFile]>) -> ())
    
    /// Get file from name.
    /// - Parameter name: File name.
    /// - Returns: Basefile or error.
    func searchFile(containName: String, completion: @escaping (SDMLResult<[SDMLBaseFile]>) -> ())
    
    /// Delete file in cache.
    /// - parameter file: Cache file struct.
    /// - returns: Nil or error.
    func deleteFile(file: SDMLBaseFile) -> SDMLResult<Any?>
    
    /// Gets the all local project file.
    ///
    /// - Parameters:
    ///   - parentPath: default "/"
    ///   - completion: files
   // func getAllLocalProjectFile(parentPath: String, completion: @escaping (SDMLResult<[SDMLLocalProjectFile]>) -> ())
    
    /// Gets local project files on the target file.
    ///
    /// - Parameters:
    ///   - targetFile: project or projectFile
    ///   - completion: local files
    func getOutBoxFileUnder(folder: SDMLProjectFile, completion: @escaping (SDMLResult<[SDMLOutBoxFile]>) -> ()) 

    /// Decrypt cahce file to folder.
    /// - parameter file: File struct.
    /// - parameter toFolder: Where decrypt file store.
    /// - Parameter isOverwrite: Overwrite file or not.
    /// - parameter completion: Decrypt path or error.
    func decryptFile(file: SDMLNXLFile, toFolder: String, isOverwrite: Bool, completion: @escaping (SDMLResult<String>) -> ())
    
    /// Share recipients.
    /// - Parameter file: Nxl file.
    /// - Parameter newRecipients: New recipients.
    /// - Parameter comment: comment for share.
    /// - Parameter completion: Success or fail.
    func reshare(file: SDMLNXLFile, newRecipients: [String], comment: String, completion: @escaping (SDMLResult<Any?>) -> ())
    
}
