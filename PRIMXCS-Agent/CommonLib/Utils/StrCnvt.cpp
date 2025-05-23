/**
@file StrCnvt.cpp
@brief 유니코드와 멀티버이트 스트링의 상호 변환 클래스의 CPP 파일.
@date 2010/04/01
@version 0.1
@author hylee
*/

#include "stdafx.h"
#include "StrCnvt.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
AtoWConverter::AtoWConverter(LPCSTR s)
{
	if (NULL == s){
		_wstr = new WCHAR[sizeof(TCHAR)];
		wmemset(_wstr, 0, sizeof(TCHAR));
		return;
	}
	int len = MultiByteToWideChar(CP_OEMCP, 0, s, -1, NULL, 0);
	_wstr = new WCHAR[len + sizeof(TCHAR)];
	wmemset(_wstr, 0, len);
	//                   CP_ACP
	MultiByteToWideChar(CP_OEMCP, 0, s, -1, _wstr, len);
}

AtoWConverter::AtoWConverter(LPCSTR s, int nLen)
{
	if (NULL == s){
		_wstr = new WCHAR[sizeof(TCHAR)];
		wmemset(_wstr, 0, sizeof(TCHAR));
		return;
	}

	char* cTemp = new char[nLen + 1];
	memset(cTemp, 0x00, nLen + 1);
	memcpy(cTemp, s, nLen);

	int len = MultiByteToWideChar(CP_OEMCP, 0, cTemp, -1, NULL, 0);
	_wstr = new WCHAR[len + sizeof(TCHAR)];
	wmemset(_wstr, 0, len);
	//                   CP_ACP
	MultiByteToWideChar(CP_OEMCP, 0, cTemp, -1, _wstr, len);
	delete[] cTemp;

}

AtoWConverter::~AtoWConverter () 
{
	_SafeDeleteArray( _wstr);
}

AtoWConverter::operator LPWSTR () 
{
	return _wstr;
}

WtoAConverter::WtoAConverter (LPCWSTR w) 
{
	// We need to convert the Unicode to ASCII string and copy after that.
	//                             CP_ACP
	int len = WideCharToMultiByte (CP_OEMCP, 0, w, -1, NULL, 0, NULL, NULL);
	_astr = new char[len+1];
	memset(_astr, 0, len);

	//                   CP_ACP
	WideCharToMultiByte (CP_OEMCP, 0, w, -1, _astr, len, NULL, NULL);
}

WtoAConverter::~WtoAConverter () 
{
	_SafeDeleteArray( _astr);
}

WtoAConverter::operator LPSTR () 
{
	return _astr;
}



WtoUtf8Converter::WtoUtf8Converter (LPCWSTR w) 
{
	// We need to convert the Unicode to ASCII string and copy after that.
	//                             CP_ACP
	int len = WideCharToMultiByte (CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);
	_astr = new char[len+1];
	memset(_astr, 0, len);

	//                   CP_ACP
	WideCharToMultiByte (CP_UTF8, 0, w, -1, _astr, len, NULL, NULL);
}

WtoUtf8Converter::~WtoUtf8Converter () 
{
	_SafeDeleteArray( _astr);
}

WtoUtf8Converter::operator LPSTR () 
{
	return _astr;
}

Utf8toWConverter::Utf8toWConverter (LPCSTR s) 
{
	int len = MultiByteToWideChar (CP_UTF8, 0, s, -1, NULL, 0);
	_wstr = new WCHAR[len+sizeof(TCHAR)];
	wmemset(_wstr, 0, len);
	//                   CP_ACP
	MultiByteToWideChar (CP_UTF8, 0, s, -1, _wstr, len);
}
Utf8toWConverter::Utf8toWConverter(LPCSTR s ,int nLen)
{
	if (NULL == s){
		_wstr = new WCHAR[sizeof(TCHAR)];
		wmemset(_wstr, 0, sizeof(TCHAR));
		return;
	}

	char* cTemp = new char[nLen + 1];
	memset(cTemp, 0x00, nLen + 1);
	memcpy(cTemp, s, nLen);
	
	int len = MultiByteToWideChar(CP_UTF8, 0, cTemp, -1, NULL, 0);
	_wstr = new WCHAR[len + sizeof(TCHAR)];
	wmemset(_wstr, 0, len);
	//                   CP_ACP
	MultiByteToWideChar(CP_UTF8, 0, cTemp, -1, _wstr, len);
}
Utf8toWConverter::~Utf8toWConverter () 
{
	_SafeDeleteArray( _wstr);
}

Utf8toWConverter::operator LPWSTR () 
{
	return _wstr;
}


