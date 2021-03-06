/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumCat.h
* @brief	This Program is PlumCat DLL Project.
* @author	alopex
* @version	v1.00a
* @date		2018-07-02	v1.00a	alopex	Create Project.
*/
#pragma once

#ifndef __PLUMCAT_H_
#define __PLUMCAT_H_

//Include Windows Header File
#include <WinSock2.h>
#include <HttpExt.h>
#include <windef.h>
#include <wincrypt.h>
#include <nb30.h>

//Include C/C++ Run Header File
#include <stdio.h>
#include <stdlib.h>

//Include Crypt File
#include "PlumCipherA.h"
#include "PlumCrypt.h"
#include "PlumPack.h"

//Include Common Library
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "netapi32.lib")

//Macro Definition
#ifdef	PLUMCAT_EXPORTS
#define PLUMCAT_API	__declspec(dllexport)
#else
#define PLUMCAT_API	__declspec(dllimport)
#endif

#define PLUMCAT_CALLMODE	__stdcall

//Struct Definition

//Class Definition
class PLUMCAT_API CPlumCat
{
private:
	CHAR m_chMAC[MAX_PATH];

protected:
	UCHAR PLUMCAT_CALLMODE PlumCatGetMAC();

public:
	UCHAR PLUMCAT_CALLMODE PlumCatGetMAC(UCHAR chMAC[6]);

public:
	CPlumCat();
	~CPlumCat();

	virtual void PLUMCAT_CALLMODE PlumCatEnCryptMAC(const char* pFile);
	virtual void PLUMCAT_CALLMODE PlumCatEnCryptMACByPath(const char* pPath);
	virtual BOOL PLUMCAT_CALLMODE PlumCatDeCryptMAC(const char* pFile);
	virtual BOOL PLUMCAT_CALLMODE PlumCatDeCryptMACbyPath(const char* pPath);

	virtual void PLUMCAT_CALLMODE PlumCatEnCryptMAC(char* pMAC, int nSize, const char* pFile);
	virtual void PLUMCAT_CALLMODE PlumCatEnCryptMACByPath(char* pMAC, int nSize, const char* pPath);

};


#endif // !__PLUMCAT_H_

