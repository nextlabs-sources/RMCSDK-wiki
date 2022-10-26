//
//  CustomTableCellView.swift
//  RMCAPITest
//
//  Created by Paul (Qian) Chen on 19/04/2018.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import Cocoa

class CustomTableCellView: NSTableCellView {

    var test: RMCAPITest?
    
    @IBOutlet weak var subheadTextField: NSTextField!
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
    }
    
    @IBAction func showOperations(_ sender: Any) {
        
        let storyboard = NSStoryboard(name: NSStoryboard.Name.init(rawValue: "Main"), bundle: nil)
        if let viewController = storyboard.instantiateController(withIdentifier: NSStoryboard.SceneIdentifier.init(rawValue: "OperationViewController")) as? OperationViewController {
            viewController.sessionId = subheadTextField.stringValue
            
            viewController.test = test
            
            let popover = NSPopover()
            popover.contentViewController = viewController
            
            let btn = sender as! NSButton
            
            popover.show(relativeTo: btn.bounds, of: btn, preferredEdge: NSRectEdge.maxX)
        }
    }
}
