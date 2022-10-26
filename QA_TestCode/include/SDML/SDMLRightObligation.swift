//
//  SDMLRightObligation.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 31/07/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public class SDMLRightObligation {
    
    public init(rights: [SDMLFileRight], watermark: SDMLWatermark?, printWatermark: SDMLWatermark? = nil, expiry: SDMLExpiry?) {
        self.rights = rights
        self.watermark = watermark
        self.printWatermark = printWatermark
        self.expiry = expiry
    }
    
    public private(set) var rights: [SDMLFileRight]
    public internal(set) var watermark: SDMLWatermark?
    public private(set) var printWatermark: SDMLWatermark?
    public private(set) var expiry: SDMLExpiry?
}


