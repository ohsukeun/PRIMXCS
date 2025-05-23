/**
@file Base64.h
@brief Defines the interface for a simple base64 decoding class
@date 2010/04/01
@version 0.1
@author hylee
*/

/////////////////////////////// Defines ///////////////////////////////////////

#ifndef __BASE64_H__
#define __BASE64_H__


/////////////////////////////// Classes ///////////////////////////////////////
/// @class CBase64
/// @brief Defines the interface for a simple base64 decoding class
class CBase64
{
public:
//Defines
  #define BASE64_FLAG_NONE	  0
  #define BASE64_FLAG_NOPAD	  1
  #define BASE64_FLAG_NOCRLF  2

//Methods
	/**
	@brief	Decode시 Decode된 데이터의 길이를 확인하는 함수
	@return	Decode시 데이터 길이
	*/
  int  DecodeGetRequiredLength(int nSrcLen /**< Decode할 데이터의 길이 */);

  /**
  @brief Encode시 Encode된 데이터의 길이를 확인하는 함수
  @return Encode시 데이터 길이
  */
  int  EncodeGetRequiredLength(int nSrcLen /**< Encode할 데이터의 길이 */, DWORD dwFlags = BASE64_FLAG_NOCRLF /**< Encode Flag */);

  /**
  @brief Encode 함수
  @return 성공여부
  */
  BOOL Encode(const BYTE* pbSrcData /**< Encode할 데이터 */, int nSrcLen /**< Encode할 데이터의 길이 */,	LPSTR szDest /**< Encode된 데이터 */,	int* pnDestLen  /**< Encode된 데이터 길이 */,	DWORD dwFlags = BASE64_FLAG_NOCRLF /**< Encode Flag */);

  /**
  @brief Decode 함수
  @return 성공여부
  */
  BOOL Decode(LPCSTR szSrc /**< Decode할 데이터 */, int nSrcLen /**< Decode할 데이터의 길이 */, BYTE* pbDest /**< Decode된 데이터 */, int* pnDestLen /**< Decode돈 데이터의 길이 */);

protected:
  int DecodeChar(unsigned int ch);
};

#endif //__BASE64_H__
