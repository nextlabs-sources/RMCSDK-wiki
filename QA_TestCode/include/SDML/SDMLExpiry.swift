//
//  SDMLExpiry.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 15/06/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

@objc public enum SDMLExpirationType: Int {
    case neverExpire
    case relativeExpire
    case absoluteExpire
    case rangeExpire
}

public class SDMLExpiry: NSObject, NSCoding {
    
    private struct Constant {
        static let kType = "type"
        static let kStart = "start"
        static let kEnd = "end"
    }
    
    public init(type: SDMLExpirationType = .neverExpire, start: UInt64 = 0, end: UInt64 = 0) {
        self.type = type
        self.start = start
        self.end = end
    }
     
    public internal(set) var type: SDMLExpirationType
    public internal(set) var start: UInt64
    public internal(set) var end: UInt64
    
    public func encode(with aCoder: NSCoder) {
        aCoder.encode(type.rawValue, forKey: Constant.kType)
        aCoder.encode(start, forKey: Constant.kStart)
        aCoder.encode(end, forKey: Constant.kEnd)
    }
    
    public required init?(coder aDecoder: NSCoder) {
        let type = aDecoder.decodeInteger(forKey: Constant.kType)
        self.type = SDMLExpirationType(rawValue: type) ?? .neverExpire
        self.start = (aDecoder.decodeObject(forKey: Constant.kStart) as? UInt64) ?? 0
        self.end = (aDecoder.decodeObject(forKey: Constant.kEnd) as? UInt64) ?? 0
    }
}
