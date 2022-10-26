//
//  sdkerror.h
//  rmsdk
//
//  Created by NextLabs Inc on 11/21/17.
//  Copyright © 2017 NextLabs, Inc. All rights reserved.
//

#pragma once
#ifndef __RMSDK_BASE_ERRORCODE_H__
#define __RMSDK_BASE_ERRORCODE_H__

#define RMSDK_ERROR_SUCCESS                 0l
#define RMSDK_ERROR_FILE_NOT_FOUND          2l      //ERROR_FILE_NOT_FOUND
#define RMSDK_ERROR_ACCESS_DENIED           5l      //ERROR_ACCESS_DENIED
#define RMSDK_ERROR_NOT_ENOUGH_MEMORY       8l		//ERROR_NOT_ENOUGH_MEMORY
#define RMSDK_INVALID_JSON_FORMAT           11l		//ERROR_BAD_FORMAT
#define RMSDK_INVALID_NXL_FORMAT            11l     //ERROR_BAD_FORMAT
#define RMSDK_ERROR_INVALID_DATA            13l		//ERROR_INVALID_DATA
#define RMSDK_ERROR_NOT_READY               21l     //ERROR_NOT_READY
#define RMSDK_ERROR_END_OF_FILE             38l	    //ERROR_HANDLE_EOF
#define RMSDK_ERROR_NOT_SUPPORTED           50l     //ERROR_NOT_SUPPORTED
#define RMSDK_ERROR_CREATE_NXL				82l		//ERROR_CANNOT_MAKE
#define RMSDK_ERROR_ALREADY_OPENED			183l	//ERROR_ALREADY_EXISTS
#define RMSDK_ERROR_NOTFOUND				1168l	//ERROR_NOT_FOUND

/************************************************************************/
/* RMS Error Code base starts from 0xF000                               */
/* for API return code from RMS, use the error code minus Error code    */
/*   base to get the result.                                            */
/*   For example, error code 61940 means 500 (61940-0xF000)             */
/************************************************************************/
#define RMSDK_RMS_ERRORCODE_BASE            0xF000

#endif /* __RMSDK_BASE_ERRORCODE_H__ */
