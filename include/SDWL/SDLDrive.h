/*!
 * \file SDLDrive.h
 *
 * \author hbwang
 * \date April 2018
 *
 * 
 */
#include <string>

#pragma once
class ISDRmMyDrive
{
public:
	ISDRmMyDrive() {};
	virtual ~ISDRmMyDrive() {};

public:
	virtual uint64_t GetUsage(void) = 0;
	/// Get usage of MyDrive repository
	/**
	* @return
	*    bytes of usage of MyDrive repository
	*/
	virtual uint64_t GetQuota(void) = 0;
	/// Get total storage of MyDrive repository
	/**
	* @return
	*    bytes of total storage for MyDrive repository
	*/

};
