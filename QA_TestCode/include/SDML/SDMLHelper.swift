//
//  SDMLHelper.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 18/05/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

/// Features with no relationship with session.
public class SDMLHelper {
    
    ///  Is nxl file.
    /// - parameter file: Nxl model.
    /// - returns: True or false.
    public static func isNXLFile(path: String) -> Bool {
        return CommonUtil.isNXLFile(path: path)
    }

}
