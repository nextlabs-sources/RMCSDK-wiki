//
//  SDMLLogManagerInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 29/03/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

/// Log manager.
public protocol SDMLLogManagerInterface {
    /// Get nxl file activity logs.
    /// - Parameter file: NXL file.
    /// - Returns: Logs or error.
    func getLogList(file: SDMLNXLFile, completion: @escaping (SDMLResult<[SDMLActivityLogInterface]>) -> ())
    
    /// Add log.
    /// - Parameter file: NXL file.
    /// - Parameter log: log to add.
    /// - Parameter completion: nil or error.
    func addLog(file: SDMLNXLFile, log: SDMLActivityLogInterface, completion: @escaping (SDMLResult<Any?>) -> ())
}
