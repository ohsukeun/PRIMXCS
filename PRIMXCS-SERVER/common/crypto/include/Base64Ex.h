/**
@file Base64Ex.h
@brief Defines the interface for a simple base64 decoding class
@date 2010/04/01
@version 0.1
*/

#include <swc.h>

/////////////////////////////// Defines ///////////////////////////////////////
#ifndef __BASE64EX_H__
#define __BASE64EX_H__

/////////////////////////////// Classes ///////////////////////////////////////
/// @class CBase64Ex
/// @brief Defines the interface for a simple base64 decoding class
class CBase64Ex
{
public:
//Defines
  #define BASE64_FLAG_NONE	  0
  #define BASE64_FLAG_NOPAD	  1
  #define BASE64_FLAG_NOCRLF  2

//Methods
	/**
	@brief get decoded data length
	@return	decoded data length
	*/
  int  DecodeGetRequiredLength(int nSrcLen /**< encoded data length */);

  /**
  @brief get encoded data length
  @return encoded data length
  */
  int  EncodeGetRequiredLength(int nSrcLen /**< raw data length */, DWORD dwFlags = BASE64_FLAG_NONE /**< Encode Flag */);

  /**
  @brief encode data
  @return result
  */
  BOOL Encode(const BYTE* pbSrcData /**< raw data */, int nSrcLen /**< raw data length */, PCHAR szDest /**< encoded data */, int* pnDestLen  /**< encoded data length */, DWORD dwFlags = BASE64_FLAG_NONE /**< Encode Flag */);

  /**
  @brief decode data
  @return result
  */
  BOOL Decode(PCHAR szSrc /**< encoded data */, int nSrcLen /**< encoded data length */, BYTE* pbDest /**< decoded data */, int* pnDestLen /**< decoded data length */);

protected:
  int DecodeChar(unsigned int ch);
};

#endif //__BASE64EX_H__
