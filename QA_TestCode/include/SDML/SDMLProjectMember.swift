//
//  SDMLProjectMember.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/1/30.
//  Copyright © 2019 NextLabs. All rights reserved.
//
import Foundation
public protocol SDMLProjectMemberInterface:SDMLUserInfoInterface {

}
public class SDMLProjectMember: SDMLUserInfo {
//    var project: SDMLProject?
    var projectId: Int?
    
    func getProjectId() -> Int {
        return projectId ?? 0
    }
}

extension SDMLProjectMember: SDMLProjectMemberInterface {
  
}
