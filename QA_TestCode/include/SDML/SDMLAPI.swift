//
//  SDLAPI.swift
//  SDML
//
//  Created by Walt (Shuiping) Li on 13/12/2017.
//  Copyright © 2017 NextLabs. All rights reserved.
//

import Foundation

/// Get library version.
public func getVersion() -> String {
    return "\(SDMLVersionNumber)"
}

/// Create session instance.
/// - parameter mark: use to distinct sessions, need to be unique.

public func createInstance(identifier: String = "") -> SDMLResult<SDMLSessionInterface> {
    _ = JavaPCXPCSingle
    let instance = SDMLSession(sessionId: identifier)
    return .success(instance)
}

/// Delete session instance.
/// - parameter session: session instance.
public func deleteInstance(session: inout SDMLSessionInterface?) {
    session = nil
}

