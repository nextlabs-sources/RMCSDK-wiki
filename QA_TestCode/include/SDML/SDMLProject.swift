//
//  SDMLProject.swift
//  SDML
//
//  Created by William (Weiyan) Zhao on 2019/1/26.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLProjectInterface {

    /// get the TotalMembers
    ///
    /// - Returns: the Project TotalMembers
    func  getTotalMembers() -> Int

    /// get the Project DisplayName
    ///
    /// - Returns: the project displayName
    func  getDisplayName() -> String

    /// get the invitationMsg
    ///
    /// - Returns: the invitationMsg
    func  getDefaultInvitationMsg() -> String

    /// get the ProjectDescription
    ///
    /// - Returns: the projectDescription
    func  getDescription() -> String

    /// get the projectId
    ///
    /// - Returns: the projectId
    func  getId() -> Int

    /// judge the file weather is Me
    ///
    /// - Returns: the judge result
    func  isOwnedByMe() -> Bool

    /// get the createTime
    ///
    /// - Returns: the creation Time
    func  getCreationTime() -> Date

    /// get the trialEndTime
    ///
    /// - Returns:  the trialEndTime
    func  getTrialEndTime() -> Date


    /// get the file owner
    ///
    /// - Returns: the file owner
    func  getOwner() -> SDMLUserInfo

    /// get the projectMember
    ///
    /// - Returns: the projectMember
    func getMembers() -> [SDMLProjectMember]
    
    /// get projectName
    ///
    /// - Returns: projectName
    func getName() -> String
    /// get the project Manager
    ///
    /// - Returns: the project manager
   // func getFileManager() -> SDMLProjectFileManager

    /// get the parentTenantId
    ///
    /// - Returns: return the parentTenantId
    func getParentTenantId() -> String

    /// get the getParentTenantName
    ///
    /// - Returns: TenantName
    func getParentTenantName() -> String
    /// get the TokenGroupName
    ///
    /// -Return: the tokenGroupName
    func getTokenGroupName() -> String
    /// Gets project tag
    ///
    /// - Returns: SDMLProjectTag
    func getTagTemplate() -> SDMLProjectTagTemplate
    
    /// get project pathId
    ///
    /// - Returns: the projectId
    func getPathId() -> String
    
    /// get projectTags form coreData
    ///
    /// - Returns: the projectTags form coreData
    func getProjectTags() -> SDMLProjectTagTemplate?
}

public class SDMLProject {
    var parentTenantName:String?
    var parentTenantId:String?
    var tokenGroupName:String?
    var totalFiles: Int?
    var totalMembers: Int?
    var converted_path: String?
    var displayName: String?
    var invitationMsg: String?
    var projectDescription: String?
    var projectId: Int?
    var membershipId: String?
    var ownedByMe: Bool?
    var creationTime:Date?
    var trialEndTime:Date?
    var accountType:String?
    var projectName:String?
    var pathId:String?
    var owner:SDMLUserInfo?
    var session:SDMLSession?
    var projectInvitation:[SDMLProjectInvitation]?
    var extraInfo:[String:Any]?
    var projectMemabers: [SDMLProjectMember]?
    var memberManager:SDMLProjectMemberManager?
    var fileManager:SDMLProjectFileManager?
    var isRemove:Bool?
    var files: [SDMLProjectFile]?
    var projectTag: SDMLProjectTagTemplate?
    
    
    func getTotalFiles() -> Int {
        return totalFiles ?? 0
    }
    
    func getRootFiles() -> [SDMLProjectFile]? {
        return files ?? [SDMLProjectFile]()
    }
    
    func getMembershipId() -> String? {
        return membershipId ?? ""
    }
}

extension SDMLProject: SDMLProjectInterface {
    public func getProjectTags() -> SDMLProjectTagTemplate? {
        return  self.session?.dbManager?.getProjectPolicy(project: self)
    }
    
    public func getPathId() -> String {
        return pathId ?? ""
    }
    
    public func getName() -> String {
        return projectName ?? ""
    }
    
    public func getParentTenantId() -> String {
        return parentTenantId ?? ""
    }
    
    public func getParentTenantName() -> String {
        return parentTenantName ?? ""
    }
    
    public func getTokenGroupName() -> String {
        return tokenGroupName ?? ""
    }
    
    
    public func getTagTemplate() -> SDMLProjectTagTemplate {
        return projectTag ?? SDMLProjectTagTemplate()
    }
    
    public func getMembers() -> [SDMLProjectMember] {
        return projectMemabers ?? [SDMLProjectMember]()
    }
    
    public func getFileManager() -> SDMLProjectFileManager {
        if fileManager == nil && self.session != nil {
//            let rootFile =  files?.filter({ projectFile -> Bool in
//                     projectFile.isFolder!
//            })
           fileManager = SDMLProjectFileManager(session: self.session!, project: self)
        }
        
        return fileManager!
    }

    public func getTrialEndTime() -> Date {
        return trialEndTime ?? Date()
    }
    
    public func getOwner() -> SDMLUserInfo {
        return owner ?? SDMLUserInfo()
    }
    
    public func getTotalMembers() -> Int {
        return totalMembers ?? 0
    }
    
    
    public func getDisplayName() -> String {
        return displayName ?? ""
        
    }
    public func getDefaultInvitationMsg() -> String {
         return invitationMsg ?? ""
    }
    
    public func getDescription() -> String {
        return projectDescription ?? ""
    }
    
    public func getId() -> Int {
         return projectId ?? 0
    }
    
    public func isOwnedByMe() -> Bool {
        return  ownedByMe ?? false
    }
    
    public func getCreationTime() -> Date {
         return creationTime ?? Date()
    }
}

