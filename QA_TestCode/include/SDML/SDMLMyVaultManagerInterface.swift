//
//  SDMLMyVaultInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 11/04/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLMyVaultManagerInterface {

    /// get file list
    ///
    /// - Parameters:
    ///   - parentPath: parent path,  'default' use "/"
    ///   - fromServer: true means from server ’default‘ is false
    ///   - isLocation: true means only get these files which have been downloaded
    /// - Returns: return the myVaultFile list
    func getFiles(_ parentPath: String, _ fromServer: Bool, _ isLocation:Bool, completion: @escaping (SDMLResult<[SDMLMyVaultFile]>) -> ())
    
    /// make the myvault file offline
    ///
    /// - Parameters:
    ///   - file: the shareWithMe file
    ///   - completion: error or SDMLShareWithMeFile
    ///   - Returns: uuid
    func makeFileOffline(_ file: SDMLMyVaultFile, completion: @escaping (SDMLResult<Any?>) -> ()) -> String

    /// make file unavailable offline
    ///
    /// - Parameter file: the file
    /// - Returns: file or error
    func makeFileUnoffline(file: SDMLMyVaultFile) -> SDMLResult<Any?>
    
    /// this method is called when viewing the document online
    ///
    /// - Parameters:
    ///   - file: the file to manipulate
    ///   - progress: progress
    ///   - completion: data
    /// - Returns: uuid
    func viewFileOnline(file: SDMLMyVaultFile, progress: ((Double) -> ())? , completion: @escaping (SDMLResult<Any?>) -> ()) -> String
    
    /// make file  
    ///
    /// - Parameter file: the file
    /// - Returns: the file make available offline
    //  func makeFileOffline(file: SDMLMyVaultFile, completion:@escaping(SDMLResult<[SDMLMyVaultFile]>)->())
    ///  Get storage
    ///
    /// - Parameter completion: Return usage and quota
    func getStorage(completion: @escaping (SDMLResult<(usage: UInt, quota: UInt)>) -> ())
    
    /// get file from name.
    /// - Parameter name: File name
    /// - Returns: Basefile or error
    func searchFile(containName: String, completion: @escaping (SDMLResult<[SDMLMyVaultFile]>) -> ())
   
}
