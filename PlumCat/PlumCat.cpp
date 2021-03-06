/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumCat.cpp
* @brief	This Program is PlumCat DLL Project.
* @author	alopex
* @version	v1.00a
* @date		2018-07-02	v1.00a	alopex	Create Project.
*/
#include "stdafx.h"
#include "PlumCat.h"

//PlumCat Class is Use for Crypt exe.(PlumCat加密猫用于exe文件加密)

//------------------------------------------------------------------
// @Function:	 CPlumCat()
// @Purpose: CPlumCat构造函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumCat::CPlumCat()
{
	memset(m_chMAC, 0, MAX_PATH);
}

//------------------------------------------------------------------
// @Function:	 ~CPlumCat()
// @Purpose: CPlumCat析构函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumCat::~CPlumCat()
{
}

//------------------------------------------------------------------
// @Function:	 PlumCatGetMAC()
// @Purpose: CPlumCat获取MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
UCHAR PLUMCAT_CALLMODE CPlumCat::PlumCatGetMAC()
{
	NCB ncb = { 0 };
	UCHAR uRet = 0;

	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, *PASTAT;
	ASTAT Adapter;

	typedef struct _LANA_ENUM
	{
		UCHAR length;
		UCHAR lana[MAX_LANA];
	}LANA_ENUM;
	LANA_ENUM lana_enum;

	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));

	memset(m_chMAC, 0, MAX_PATH);

	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);

	uRet = Netbios(&ncb);
	if (uRet != NRC_GOODRET)
	{
		return uRet;
	}

	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRet = Netbios(&ncb);
	}
	if (uRet != NRC_GOODRET)
	{
		return uRet;
	}

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);

	uRet = Netbios(&ncb);

	if (uRet != NRC_GOODRET)
		return uRet;

	sprintf(m_chMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]
	);

	return 0;
}

//------------------------------------------------------------------
// @Function:	 PlumCatGetMAC()
// @Purpose: CPlumCat获取MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
UCHAR PLUMCAT_CALLMODE CPlumCat::PlumCatGetMAC(UCHAR chMAC[6])
{
	NCB ncb = { 0 };
	UCHAR uRet = 0;

	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, *PASTAT;
	ASTAT Adapter;

	typedef struct _LANA_ENUM
	{
		UCHAR length;
		UCHAR lana[MAX_LANA];
	}LANA_ENUM;
	LANA_ENUM lana_enum;

	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));

	memset(m_chMAC, 0, MAX_PATH);

	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);

	uRet = Netbios(&ncb);
	if (uRet != NRC_GOODRET)
	{
		return uRet;
	}

	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRet = Netbios(&ncb);
	}
	if (uRet != NRC_GOODRET)
	{
		return uRet;
	}

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);

	uRet = Netbios(&ncb);

	if (uRet != NRC_GOODRET)
		return uRet;

	for (int i = 0; i < 6; ++i)
	{
		chMAC[i] = Adapter.adapt.adapter_address[i];
	}

	return 0;
}

//------------------------------------------------------------------
// @Function:	 PlumCatEnCryptMAC()
// @Purpose: CPlumCat加密MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void PLUMCAT_CALLMODE CPlumCat::PlumCatEnCryptMAC(const char* pFile)
{
	CPlumPack* pPlumPack = NULL;
	HCRYPTPROV hCryptProv = NULL;
	DWORD dwLuckyArr[4] = { 0 };
	CHAR chFile[MAX_PATH] = { 0 };
	CHAR* pTemp = NULL;

	pPlumPack = new CPlumPack();

	PlumCatGetMAC();	//获取MAC地址

	memset(chFile, 0, MAX_PATH);
	GetModuleFileNameA(NULL, chFile, MAX_PATH);
	pTemp = strrchr(chFile, '\\');
	if (pTemp != NULL)
	{
		*pTemp = '\0';
	}

	strcat_s(chFile, "\\");
	strcat_s(chFile, pFile);

	CryptAcquireContext((HCRYPTPROV*)&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[0]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[1]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[2]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[3]));
	pPlumPack->PlumPackFileExtractFromMemoryA(m_chMAC, strlen(m_chMAC), chFile, dwLuckyArr);

	if (hCryptProv != NULL)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	SAFE_DELETE(pPlumPack);
}

//------------------------------------------------------------------
// @Function:	 PlumCatEnCryptMAC()
// @Purpose: CPlumCat加密MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void PLUMCAT_CALLMODE CPlumCat::PlumCatEnCryptMACByPath(const char * pPath)
{
	CPlumPack* pPlumPack = NULL;
	HCRYPTPROV hCryptProv = NULL;
	DWORD dwLuckyArr[4] = { 0 };

	pPlumPack = new CPlumPack();

	PlumCatGetMAC();	//获取MAC地址

	CryptAcquireContext((HCRYPTPROV*)&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[0]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[1]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[2]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[3]));
	pPlumPack->PlumPackFileExtractFromMemoryA(m_chMAC, strlen(m_chMAC), pPath, dwLuckyArr);

	if (hCryptProv != NULL)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	SAFE_DELETE(pPlumPack);
}

//------------------------------------------------------------------
// @Function:	 PlumCatGetMAC()
// @Purpose: CPlumCat解密MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
BOOL PLUMCAT_CALLMODE CPlumCat::PlumCatDeCryptMAC(const char* pFile)
{
	BOOL bRet = FALSE;
	CPlumPack* pPlumPack = NULL;
	CHAR chFile[MAX_PATH] = { 0 };
	CHAR* pTemp = NULL;

	pPlumPack = new CPlumPack();

	PlumCatGetMAC();	//获取MAC地址

	memset(chFile, 0, MAX_PATH);
	GetModuleFileNameA(NULL, chFile, MAX_PATH);
	pTemp = strrchr(chFile, '\\');
	if (pTemp != NULL)
	{
		*pTemp = '\0';
	}

	strcat_s(chFile, "\\");
	strcat_s(chFile, pFile);

	CHAR chMAC[MAX_PATH] = { 0 };
	pPlumPack->PlumUnPackFileStoreInMemoryA(chFile, chMAC, MAX_PATH);

	if (!strcmp(m_chMAC, (const char*)chMAC))
	{
		bRet = TRUE;
	}

	SAFE_DELETE(pPlumPack);

	return bRet;
}

//------------------------------------------------------------------
// @Function:	 PlumCatGetMAC()
// @Purpose: CPlumCat解密MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
BOOL PLUMCAT_CALLMODE CPlumCat::PlumCatDeCryptMACbyPath(const char * pPath)
{
	BOOL bRet = FALSE;
	CPlumPack* pPlumPack = NULL;

	pPlumPack = new CPlumPack();

	PlumCatGetMAC();	//获取MAC地址

	CHAR chMAC[MAX_PATH] = { 0 };
	pPlumPack->PlumUnPackFileStoreInMemoryA(pPath, chMAC, MAX_PATH);

	if (!strcmp(m_chMAC, (const char*)chMAC))
	{
		bRet = TRUE;
	}

	SAFE_DELETE(pPlumPack);

	return bRet;
}

//------------------------------------------------------------------
// @Function:	 PlumCatEnCryptMAC()
// @Purpose: CPlumCat加密MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void PLUMCAT_CALLMODE CPlumCat::PlumCatEnCryptMAC(char * pMAC, int nSize, const char* pFile)
{
	CPlumPack* pPlumPack = NULL;
	HCRYPTPROV hCryptProv = NULL;
	DWORD dwLuckyArr[4] = { 0 };
	CHAR chFile[MAX_PATH] = { 0 };
	CHAR* pTemp = NULL;

	pPlumPack = new CPlumPack();

	memset(chFile, 0, MAX_PATH);
	GetModuleFileNameA(NULL, chFile, MAX_PATH);
	pTemp = strrchr(chFile, '\\');
	if (pTemp != NULL)
	{
		*pTemp = '\0';
	}

	strcat_s(chFile, "\\");
	strcat_s(chFile, pFile);

	CryptAcquireContext((HCRYPTPROV*)&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[0]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[1]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[2]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[3]));
	pPlumPack->PlumPackFileExtractFromMemoryA(pMAC, nSize, chFile, dwLuckyArr);

	if (hCryptProv != NULL)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	SAFE_DELETE(pPlumPack);
}

//------------------------------------------------------------------
// @Function:	 PlumCatEnCryptMAC()
// @Purpose: CPlumCat加密MAC地址
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void PLUMCAT_CALLMODE CPlumCat::PlumCatEnCryptMACByPath(char * pMAC, int nSize, const char * pPath)
{
	CPlumPack* pPlumPack = NULL;
	HCRYPTPROV hCryptProv = NULL;
	DWORD dwLuckyArr[4] = { 0 };

	pPlumPack = new CPlumPack();

	CryptAcquireContext((HCRYPTPROV*)&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[0]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[1]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[2]));
	CryptGenRandom(hCryptProv, 4, (BYTE*)(&dwLuckyArr[3]));
	pPlumPack->PlumPackFileExtractFromMemoryA(pMAC, nSize, pPath, dwLuckyArr);

	if (hCryptProv != NULL)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	SAFE_DELETE(pPlumPack);
}
