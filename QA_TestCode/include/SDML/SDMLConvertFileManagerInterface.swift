//
//  SDMLConvertFileManagerInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 2018/9/11.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLConvertFileManagerInterface {
    func convert(path: String, toFormat: String, toFolder: String, isOverwrite: Bool, progress: ((Double) -> ())?, completion: @escaping (SDMLResult<String>) -> ()) -> String
    func cancel(id: String) -> Bool
}
