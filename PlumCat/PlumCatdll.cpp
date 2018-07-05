/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumCatdll.cpp
* @brief	This Program is PlumCat DLL Project.
* @author	alopex
* @version	v1.00a
* @date		2018-07-02	v1.00a	alopex	Create Project.
*/
#include "stdafx.h"
#include "PlumCatdll.h"

CPlumCat g_PlumCat;

//------------------------------------------------------------------
// @Function:	 PlumCatDllEnCrypt()
// @Purpose: PlumCatDllInitº”√‹
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void PLUMCATDLL_CALLMODE PlumCatDllEnCrypt()
{
	g_PlumCat.PlumCatEnCryptMAC("Cipher.cyt");
}

//------------------------------------------------------------------
// @Function:	 PlumCatDllDeCrypt()
// @Purpose: PlumCatDllInitΩ‚√‹
// @Since: v1.00a
// @Para: None
// @Return: BOOL
//------------------------------------------------------------------
BOOL PLUMCATDLL_CALLMODE PlumCatDllDeCrypt()
{
	BOOL bRet = FALSE;
	bRet = g_PlumCat.PlumCatDeCryptMAC("Cipher.cyt");
	return bRet;
}