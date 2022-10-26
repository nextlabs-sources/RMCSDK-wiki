//
//  NXLTest.m
//  RMDCoreSample
//
//  Created by Paul (Qian) Chen on 08/01/2018.
//  Copyright © 2018 nextlabs. All rights reserved.
//

#import "NXLTest.h"
#include "rmsdk/rmsdk.h"
#include "rmsdk/format/nxlfile.h"
#include "common.hpp"

using namespace RMSDK;
using namespace std;

static const uint8_t kTokenId[16] = {
    0x2E, 0x14, 0xD3, 0x30, 0xC7, 0xB0, 0xD5, 0xB3, 0xF9, 0x19, 0x1A, 0x46, 0x8F, 0xB7, 0xBD, 0xB8
};
static const uint8_t kTokenValue[32] = {
    0xB9, 0x1C, 0xBB, 0xB7, 0x29, 0xEB, 0x16, 0xD1, 0xF4, 0x41, 0xA2, 0x71, 0xC8, 0x25, 0xAB, 0x72,
    0x41, 0x51, 0x74, 0x33, 0xA5, 0xD7, 0x43, 0x98, 0xD1, 0xCD, 0xEB, 0x09, 0xB1, 0x98, 0xE1, 0x3A
};
static const uint32_t kTokenLevel = 0;
static const std::string kOwnerId("m14@skydrm.com");
static const std::string kRecipient("Gavin.Ye@nextlabs.com");

@implementation NXLTest

- (void)testNXLFile {
    const std::string nxlFile("RMSTokenTest.txt.nxl");
    const std::string txtFile("DecryptRMSTokenTest.txt");
    NXLFMT::File file;
    
    file.open(nxlFile, true);
    assert(0 == memcmp(kTokenId, file.getTokenId().getId().data(), 16));
    
    file.setTokenKey(std::vector<uint8_t>(kTokenValue, kTokenValue + 32));
    
    std::ofstream ifs;
    ifs.open(txtFile, ios_base::trunc | ios_base::out);
    assert(ifs.is_open());
    uint64_t contentLength = file.getContentLength();
    uint64_t offset = 0;
    
    cout << endl << "Open nxl file " << nxlFile << endl;
    cout << "File content :" << endl;
    bool beof = 0;
    while (contentLength) {
        uint8_t buf[513];
        uint32_t bytesToRead = 512;
        memset(buf, 0, sizeof(buf));
        bytesToRead = file.read(offset, buf, bytesToRead);
        if (bytesToRead > contentLength)
            bytesToRead = (uint32_t)contentLength;
        ifs.write((char *)buf, bytesToRead);
        cout << buf;
        offset += bytesToRead;
        contentLength -= bytesToRead;
    }
    file.close();
    ifs.close();
}
@end

