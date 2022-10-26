//
//  SDMLProjectFileManagerInterface.swift
//  SDML
//
//  Created by William on 29/6/2019.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation
public protocol SDMLProjectFileManagerInterface  {
    
    /// Gets project file of project
    ///
    /// - Returns: file
    func getRootFiles() -> [SDMLProjectFile]
    
    /// get the file root Folder
    ///
    /// - Returns: the root Folder
    func getRootFolder() -> SDMLProjectFile
    /// get the TotalFile
    ///
    /// - Returns: the TotalFile
    func  getTotalFiles() -> Int
    
    /// update the file of the project
    /// description: if update the file object is the folder will updat all the project
    /// - Parameters:
    ///    - project: current project
    ///   - completion: update the project
    func update(completion:@escaping(SDMLResult<SDMLProject>) -> ())
    
    /// make the file offline form the server
    ///
    /// - Parameters:
    ///   - file: the file  selected
    ///   - completion: the file have download completion
    /// - Returns: uuid of the file
    func makeFileOffline(_ file: SDMLProjectFile?, completion: @escaping (SDMLResult<Any?>) -> ()) -> String
    
    /// remove the file from disk and database
    ///
    /// - Parameter file:  the file
    /// - Returns: the result
    func makeFileUnoffline(file: SDMLProjectFile) -> SDMLResult<Any?>
    
    
    /// View file online
    ///
    /// - Parameters:
    ///   - file: the file
    ///   - progress: download progress
    ///   - completion: return call
    /// - Returns: the result
    func viewFileOnline(file: SDMLProjectFile, progress: ((Double) -> ())? , completion: @escaping (SDMLResult<Any?>) -> ()) -> String
    
    /// add the nxl file to other project
    ///
    /// - Parameters:
    ///   - file: the file
    ///   - toFolder: file decrypt save path
    ///   - totalProject: remove to the project
    ///   - completion: return the operation result
    
    /// add the nxl file to other project
    ///
    /// - Parameters:
    ///   - file: the file
    ///   - tags: the new tags
    ///   - toFolder:file decrypt save path
    ///   - systemFilePath: systembucket file paths
    ///   - totalProject: the total project
    ///   - completion: return the operation result
    func addNXLFileToProject(file:SDMLNXLFile?,tags:[String:[String]], systemFilePath:String?, goalProject:SDMLNXLFile,completion: @escaping(SDMLResult<SDMLNXLFile>)->())
    
}
