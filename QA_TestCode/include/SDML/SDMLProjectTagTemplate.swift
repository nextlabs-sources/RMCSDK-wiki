//
//  SDMLProjectTagTemplate.swift
//  SDML
//
//  Created by Ivan (Youmin) Zhang on 2019/3/11.
//  Copyright © 2019 NextLabs. All rights reserved.
//

import Foundation

public protocol SDMLProjectTagTemplateInterface {
    
}


public class SDMLProjectTagTemplate {
    var maxCategoryNum: Int?
    var    maxLabelNum: Int?
    var     categories: [SDMLTagCategory]?
}

extension SDMLProjectTagTemplate: SDMLProjectTagTemplateInterface {
    
    /// Gets category num max
    ///
    /// - Returns: int
    public func getMaxCategoryNum() -> Int {
        return maxCategoryNum ?? 0
    }
    
    
    /// Gets label num max
    ///
    /// - Returns: int
    public func getMaxLabelNum() -> Int {
        return maxLabelNum ?? 0
    }
    
    
    /// Gets categories
    ///
    /// - Returns: []
    public func getCategories() -> [SDMLTagCategory] {
        return categories ?? []
    }
}


public protocol SDMLTagCategoryInterface {
    
}


public class SDMLTagCategory {
    var        name: String?
    var multiSelect: Bool?
    var   mandatory: Bool?
    var   isDefault: Bool?
    var      labels: [SDMLTagLabel]?
}

extension SDMLTagCategory: SDMLTagCategoryInterface {
    /// Gets name
    ///
    /// - Returns: name
    public func getName() -> String {
        return name ?? ""
    }
    
    /// Is it multiple choice
    ///
    /// - Returns: bool
    public func getMultiSelect() -> Bool {
        return multiSelect ?? false
    }
    
    /// Is it required
    ///
    /// - Returns: bool
    public func getMmandatory() -> Bool {
        return mandatory ?? false
    }
    
    /// Gets labs
    ///
    /// - Returns:
    public func getLabels() -> [SDMLTagLabel] {
        return labels ?? []
    }
    
}


public protocol SDMLTagLabelInterface {
    
}


public class SDMLTagLabel {
    var      name: String?
    var isDefault: Bool?
}

extension SDMLTagLabel: SDMLTagLabelInterface {
    /// Gets name
    ///
    /// - Returns: name
    public func getName() -> String {
        return name ?? ""
    }
    
    /// Is the name whether default in labels
    ///
    /// - Returns: bool
    public func getIsDefault() -> Bool {
        return isDefault ?? false
    }
}

