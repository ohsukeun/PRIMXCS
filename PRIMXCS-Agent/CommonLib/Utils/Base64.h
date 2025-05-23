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
	@brief	Decode�� Decode�� �������� ���̸� Ȯ���ϴ� �Լ�
	@return	Decode�� ������ ����
	*/
  int  DecodeGetRequiredLength(int nSrcLen /**< Decode�� �������� ���� */);

  /**
  @brief Encode�� Encode�� �������� ���̸� Ȯ���ϴ� �Լ�
  @return Encode�� ������ ����
  */
  int  EncodeGetRequiredLength(int nSrcLen /**< Encode�� �������� ���� */, DWORD dwFlags = BASE64_FLAG_NOCRLF /**< Encode Flag */);

  /**
  @brief Encode �Լ�
  @return ��������
  */
  BOOL Encode(const BYTE* pbSrcData /**< Encode�� ������ */, int nSrcLen /**< Encode�� �������� ���� */,	LPSTR szDest /**< Encode�� ������ */,	int* pnDestLen  /**< Encode�� ������ ���� */,	DWORD dwFlags = BASE64_FLAG_NOCRLF /**< Encode Flag */);

  /**
  @brief Decode �Լ�
  @return ��������
  */
  BOOL Decode(LPCSTR szSrc /**< Decode�� ������ */, int nSrcLen /**< Decode�� �������� ���� */, BYTE* pbDest /**< Decode�� ������ */, int* pnDestLen /**< Decode�� �������� ���� */);

protected:
  int DecodeChar(unsigned int ch);
};

#endif //__BASE64_H__
