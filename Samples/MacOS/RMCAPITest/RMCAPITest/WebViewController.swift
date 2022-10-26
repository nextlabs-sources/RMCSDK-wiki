//
//  ViewController.swift
//  test
//
//  Created by Paul (Qian) Chen on 22/01/2018.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import Cocoa
import WebKit

enum WebViewType {
    case login
    case register
}

protocol WebViewControllerDelegate {
    func loginFinish(viewController: WebViewController)
}

class WebViewController: NSViewController {

    @IBOutlet weak var webView: WKWebView!
    
    struct Constant {
        static let messageHandleName = "fetchLoginResult"
    }
    
    var delegate: WebViewControllerDelegate?
    
    var sessionId: String?
    var test: RMCAPITest?
    var type: WebViewType = .login
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        createSession()
        getWebViewURLRequest()
    }
    
    func createSession() {
        let sessionId = SessionIDManager.shared.createSessionID()
        self.sessionId = sessionId
        self.test = RMCAPITest(identifier: sessionId)
    }
    
    func getWebViewURLRequest() {
        cleanCookieCache()
        
        injectJS()
        if type == .login {
            self.test?.testLogin(completion: loadWebView)
        } else {
            self.test?.testRegister(completion: loadWebView)
        }
        
    }
    
    func cleanCookieCache() {
        debugPrint(HTTPCookieStorage.shared.cookies?.count)
        if let cookies = HTTPCookieStorage.shared.cookies {
            for cookie in cookies {
                HTTPCookieStorage.shared.deleteCookie(cookie)
            }
        }
        debugPrint(HTTPCookieStorage.shared.cookies?.count)
        
        // No ruan yong.
//        let libUrl = FileManager.default.urls(for: .libraryDirectory, in: .userDomainMask).first!
//        let cookieUrl = libUrl.appendingPathComponent("/Cookies")
//        try? FileManager.default.removeItem(at: cookieUrl)
    }
    
    func injectJS() {
        let source = "var s_ajaxListener = new Object();s_ajaxListener.tempOpen = XMLHttpRequest.prototype.open;XMLHttpRequest.prototype.open = function() {this.addEventListener('readystatechange', function() {var result = eval(\"(\" + this.responseText + \")\");alert('2');if(result.statusCode == 200 && result.message == \"Authorized\") {var temp = this.responseText;window.webkit.messageHandlers.fetchLoginResult.postMessage(temp);}}, false);s_ajaxListener.tempOpen.apply(this, arguments);}"
        let script = WKUserScript(source: source, injectionTime: WKUserScriptInjectionTime.atDocumentStart, forMainFrameOnly: false)
        webView.configuration.userContentController.addUserScript(script)
        
        webView.configuration.userContentController.removeScriptMessageHandler(forName: Constant.messageHandleName)
        webView.configuration.userContentController.add(self, name: Constant.messageHandleName)
    }
    
    func loadWebView(cookie: String, urlRequest: URLRequest) {
        let cookieScript = WKUserScript(source: cookie, injectionTime: WKUserScriptInjectionTime.atDocumentStart, forMainFrameOnly: false)
        webView.configuration.userContentController.addUserScript(cookieScript)
        
        debugPrint(HTTPCookieStorage.shared.cookies?.count)
        webView.load(urlRequest)
    }
    
    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
}

extension WebViewController: WKScriptMessageHandler {
    // FIXME: call twice.
    func userContentController(_ userContentController: WKUserContentController, didReceive message: WKScriptMessage) {
        webView.configuration.userContentController.removeScriptMessageHandler(forName: Constant.messageHandleName)
        webView.configuration.userContentController.removeAllUserScripts()
        
        if let json = message.body as? String, let cookies = HTTPCookieStorage.shared.cookies {
            test?.testLoginWithResponse(cookies: cookies, str: json) {
                self.delegate?.loginFinish(viewController: self)
            }
        }
        
        
    }
}
