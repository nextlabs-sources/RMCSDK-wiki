//
//  SDLResult.swift
//  SDML
//
//  Created by Walt (Shuiping) Li on 13/12/2017.
//  Copyright © 2017 NextLabs. All rights reserved.
//

import Foundation

/// Used to represent whether a request was successful or encountered an error.
///
/// - success:
///
/// - failure:
public enum SDMLResult<Value> {
    case success(Value)
    case failure(Error)
    
    /// Returns the associated value if the result is a success, `nil` otherwise.
    public var value: Value? {
        switch self {
        case .success(let value):
            return value
        case .failure:
            return nil
        }
    }
    
    /// Returns the associated error value if the result is a failure, `nil` otherwise.
    public var error: Error? {
        switch self {
        case .success:
            return nil
        case .failure(let error):
            return error
        }
    }
}
