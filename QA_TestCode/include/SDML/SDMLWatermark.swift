//
//  SDMLWatermark.swift
//  SDML
//
//  Created by Paul (Qian) Chen on 26/04/2018.
//  Copyright © 2018 NextLabs. All rights reserved.
//

import Foundation

public enum SDMLWatermarkRotation {
    case no
    case clockwise
    case antiClockwise
}

public class SDMLWatermark {
    var text: String
    var fontName: String = ""
    var fontColor: String = ""
    var fontSize: Int = 0
    var transparency: Int = 0
    var rotation: SDMLWatermarkRotation = .no
    var isRepeat: Bool = false
    
    public init() {
        self.text = ""
    }
    
    public init(text: String) {
        self.text = text
    }
    
    public func getText() -> String {
        return text
    }
    
    public func getFontName() -> String {
        return fontName
    }
    
    public func getFontColor() -> String {
        return fontColor
    }
    
    public func getFontSize() -> Int {
        return fontSize
    }
    
    public func getTransparency() -> Int {
        return transparency
    }
    
    public func getRotation() -> SDMLWatermarkRotation {
        return rotation
    }
    
    public func getIsRepeat() -> Bool {
        return isRepeat
    }
}
