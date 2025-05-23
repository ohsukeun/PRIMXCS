/**
@file StrCnvt.h
@brief 유니코드와 멀티버이트 스트링의 상호 변환 클래스의 헤더 파일.
@date 2010/04/01
@version 0.1
@author hylee
*/

#pragma once

#include <windows.h>

/// @class AtoWConverter
/// @brief 멀티바이트 문자열을 유니코드 문자열로 변환
class  AtoWConverter 
{
	LPWSTR _wstr;	/**<  유니코드 버퍼 */
public:
	/**
	@brief	Constructor, _wstr 포인터에 메모리 할당
	@return	void
	*/
	AtoWConverter(LPCSTR s /**<  멀티바이트 버퍼 */);
	AtoWConverter(LPCSTR s /**<  멀티바이트 버퍼 */,int nLen);

	/**
	@brief	Destructor, _wstr 포인터에서 메모리 해제
	@return	void
	*/
	~AtoWConverter ();

	/**
	@brief	casting 연산자 overriding
	@return	유니코드 버퍼(_wstr)
	*/
	operator LPWSTR ();
};

/// @class WtoAConverter
/// @brief 유니코드 문자열을 멀티바이트 문자열로 변환
class  WtoAConverter 
{
	LPSTR _astr;/**<  멀티바이트 버퍼 */
public:
	/**
	@brief	Constructor, _astr 포인터에 메모리 할당
	@return	void
	*/
	WtoAConverter (LPCWSTR w /**<  유니코드 버퍼 */);

	/**
	@brief	Destructor, _astr 포인터에서 메모리 해제
	@return	void
	*/
	~WtoAConverter ();

	/**
	@brief	casting 연산자 overriding
	@return	멀티바이트 버퍼(_astr)
	*/
	operator LPSTR ();
};

/// @class WtoUtf8Converter
/// @brief 유니코드 문자열을 UTF-8 문자열로 변환
class  WtoUtf8Converter 
{
	LPSTR _astr;
public:
	WtoUtf8Converter (LPCWSTR w );
	~WtoUtf8Converter ();
	operator LPSTR ();
};

/// @class WtoUtf8Converter
/// @brief UTF-8 문자열을 유니코드 문자열로 변환
class  Utf8toWConverter 
{
	LPWSTR _wstr;	
public:
	Utf8toWConverter (LPCSTR s);
	Utf8toWConverter(LPCSTR s, int nLen);
	~Utf8toWConverter();
	operator LPWSTR ();
};

