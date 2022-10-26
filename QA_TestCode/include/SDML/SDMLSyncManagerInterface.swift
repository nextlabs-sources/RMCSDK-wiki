//
//  SDMLDownloadUploadMgrAPI.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 26/02/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

/// Manager upload.
 public protocol SDMLSyncManagerInterface {
    /**
     Upload File.
     - Parameters:
        - file: file struct.
        - progress: upload progress.
        - completion: finish or error.
     - Returns: uuid for cancel.
     */
   func upload(file: SDMLNXLFile, progress: ((Double) -> ())?, completion: @escaping (SDMLResult<Any?>) -> ()) -> String
    
   /// download file make file available offline
   ///
   /// - Parameters:
   ///   - file: file that we will get download
   ///   - progress: download progress
   ///   - completion: finish or error

   /// - Returns: uuid for cancel.
   func download(file: SDMLNXLFile, progress:((Double)->())?, completion:@escaping(SDMLResult<Any?>)->())->String


    /// Cancel upload.
    /// - Parameter id: upload id.
    /// - Returns: success or fail.
    func cancel(id: String) -> Bool
}
extension SDMLSyncManagerInterface {
    func upload(file: SDMLNXLFile, progress: ((Double) -> ())?, completion: @escaping (SDMLResult<Any?>) -> ()) -> String {
        return ""
    }
    
    func download(file: SDMLNXLFile, progress:((Double)->())?, completion:@escaping(SDMLResult<Any?>)->())->String {

        return ""
    }
    
}
