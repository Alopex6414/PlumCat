/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumCatdll.h
* @brief	This Program is PlumCat DLL Project.
* @author	alopex
* @version	v1.00a
* @date		2018-07-02	v1.00a	alopex	Create Project.
*/
#pragma once

#ifndef __PLUMCATDLL_H_
#define __PLUMCATDLL_H_

//Include PlumCat Header File
#include "PlumCat.h"

//Macro Definition
#ifdef	PLUMCAT_EXPORTS
#define PLUMCATDLL_API	extern "C"	__declspec(dllexport)
#else
#define PLUMCATDLL_API	extern "C"	__declspec(dllimport)
#endif

#define PLUMCATDLL_CALLMODE	__cdecl

//Variable Definition
PLUMCATDLL_API CPlumCat g_PlumCat;

//Function Definition
PLUMCATDLL_API void PLUMCATDLL_CALLMODE PlumCatDllEnCrypt();
PLUMCATDLL_API BOOL PLUMCATDLL_CALLMODE PlumCatDllDeCrypt();

#endif // !__PLUMCATDLL_H_
