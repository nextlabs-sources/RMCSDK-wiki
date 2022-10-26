//
//  SDMLShareManagerInterface.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2018/12/29.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLSharedWithMeManagerInterface {
    
    /// get ShareWithMeFiles
    ///
    /// - Parameters:
    ///   - parentPath: parent path, if root use "/"
    ///   - fromServer: true means from server 'default' is false
    ///   - isLocation: true means only get these files which have been downloaded
    ///   - compleation: return the ShareWithMe files
    func getFiles(_ parentPath: String, _ fromServer: Bool, isLocation: Bool,  completion: @escaping (SDMLResult<[SDMLShareWithMeFile]>) -> ())
    
    
    /// make the share with me file offline
    ///
    /// - Parameters:
    ///   - file: the shareWithMe file
    ///   - completion: error or SDMLShareWithMeFile
    ///   - Returns: uuid 
    func makeFileOffline(_ file: SDMLShareWithMeFile, completion: @escaping (SDMLResult<Any?>) -> ()) -> String
    
    /// make file unavailable offline
    ///
    /// - Parameter file: the shareWithMe file
    /// - Returns: file or error
    
    func makeFileUnoffline(_ file: SDMLShareWithMeFile) -> SDMLResult<Any?>
    
    /// this method is called when viewing the document online
    ///
    /// - Parameters:
    ///   - file: the file to manipulate
    ///   - progress: progress
    ///   - completion: data
    /// - Returns: uuid
    func viewFileOnline(file: SDMLShareWithMeFile, progress: ((Double) -> ())? , completion: @escaping (SDMLResult<Any?>) -> ()) -> String
    
    /// get search file result
    ///
    /// - Parameters:
    ///   - containName: search name
    ///   - completion: return search result
    func search(containName:String,completion:@escaping(SDMLResult<[SDMLShareWithMeFile]>)->())
    
}
