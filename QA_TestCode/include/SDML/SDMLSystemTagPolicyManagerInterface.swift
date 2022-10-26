//
//  SDMLSystemTagPolicyManagerInterface.swift
//  SDML
//
//  Created by Ivan (Youmin) Zhang on 2019/4/1.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLSystemTagPolicyManagerInterface {
    
    /// Gets system policy tags
    ///
    /// - Returns: tags model
    func getSystemPolicyTagModel() -> SDMLProjectTagTemplate?
    func saveSystemTag(categoryArr: [[String: Any]])
}


