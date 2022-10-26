//
//  SDMLProjectManagerInterface.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/1/29.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLProjectManagerInterface {
    
    /// Gets the project list.
    ///
    /// - Parameters:
    ///   - fromServer: true means from server ’default‘ is false
    ///   - completion: return the project list
    func getProjectList(_ fromServer: Bool, _ completion: @escaping (SDMLResult<[SDMLProject]>) -> ())
    
    /// Gets all isLocation file of project.
    ///
    /// - Parameter completion: files
    /// - Returns:
    func getOfflineFileOfAllProject(completion: @escaping (SDMLResult<[SDMLProjectFile]>) -> ())

    /// get the right with tags
    ///
    /// - Parameters:
    ///   - totalProject: the project
    ///   - tags: tags
    ///   - completion: return rights
    func getNXLFileObligation(totalProject:SDMLProject?,tags:[String:[String]],completion: @escaping(SDMLResult<SDMLRightObligation>?)->())
    
}
