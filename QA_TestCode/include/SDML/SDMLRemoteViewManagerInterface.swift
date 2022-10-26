//
//  SDMLRemoteViewManagerInterface.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 2018/9/11.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLRemoteViewManagerInterface {
    func viewLocal(path: String, progress: ((Double) -> ())?, completion: @escaping (SDMLResult<SDMLRemoteViewResult>) -> ()) -> String
    func cancel(id: String) -> Bool
}
