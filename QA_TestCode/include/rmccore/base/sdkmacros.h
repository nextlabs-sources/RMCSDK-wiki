//
//  sdkmacros.h
//  rmsdk
//
//  Created by Gavin Ye on 10/19/17.
//  Copyright © 2017 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMSDK_BASE_MACROS_H__
#define __RMSDK_BASE_MACROS_H__
#include <cstdint>

#ifndef Add2Ptr
#define Add2Ptr(P, O)   ((void*)((uint_ptr)(P) + (uint32_t)(O)))
#endif

#endif /* __RMSDK_BASE_MACROS_H__ */
