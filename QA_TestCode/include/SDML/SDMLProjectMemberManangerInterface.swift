//
//  SDMLProjectMemberManangerInterface.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/2/28.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation
protocol SDMLProjectMemberManagerInterface {
    /// get the project members
    ///
    /// - Parameters:
    ///   - project: the project
    ///   - completion: return the project members
    func getProjectMembers(_ project:SDMLProject,completion:@escaping(SDMLResult<SDMLProjectMember>) -> ())
}
