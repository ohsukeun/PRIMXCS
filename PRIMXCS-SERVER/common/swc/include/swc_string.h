/*****************************************************************************
 *                                                                           *
 * File Name : swc_string.h                                                  *
 * Theme     : string wrapper class                                          *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 14                     *
 *             2. ReverseFind(X, Y) methods added. '18. 06. 20               *
 *                                                                           *
 *****************************************************************************/

/// @file swc_string.h 
/// @author Park, sung soo
/// @date 2009-10-14
/// @brief std::string wrapper class

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iconv.h>

#include <swc_base.h>
//#include <swc_safty.h>

#ifndef __SWC_STRING_H__
#define __SWC_STRING_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#define STR_ITERATOR  std::string::iterator
#define STR_RITERATOR std::string::reverse_iterator
#define _STR_         std::string

// related macro definitions
#define LPCSTR(X) (const CHAR*)X.m_Str.c_str()
#define LPSTR(X)  (CHAR*)X.m_Str.c_str()

/// string class (std::string wrapper)
class WCString
{
public :
	const WCString& Append(IN const WCString& string)
	{
		m_Str += string.m_Str;
		return *this;
	};
	const WCString& Append(IN CHAR ch)
	{
		m_Str += ch;
		return *this;
	};
	const WCString& Append(IN const CHAR* lpsz)
	{
		if(NULL == lpsz) return *this;
		m_Str += lpsz;
		return *this;
	};
	INT Compare(IN const CHAR* lpsz) const
	{
		if(NULL == lpsz) return 0;
		return m_Str.compare(lpsz);
	};
	INT Compare(IN _STR_ str) const
	{
		return m_Str.compare(str);
	};
	INT Compare(IN const WCString& str) const
	{
		return m_Str.compare(str.m_Str);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const WCString& str) const
	{
		return m_Str.compare((size_t)nIndex, (size_t)nCount, str.m_Str);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const CHAR* str) const
	{
		if(NULL == str) return 0;
		return m_Str.compare((size_t)nIndex, nCount, str);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const WCString& str, IN INT nIndex2, IN INT nCount2) const
	{
		return m_Str.compare((size_t)nIndex, (size_t)nCount, str.m_Str, (size_t)nIndex2, (size_t)nCount2);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const CHAR* str, IN INT nCount2) const
	{
		if(NULL == str) return 0;
		return m_Str.compare((size_t)nIndex, (size_t)nCount, str, (size_t)nCount2);
	};
	INT CompareNoCase(IN const CHAR* lpsz) const
	{
		if(NULL == lpsz) return 1;
		return (INT)strcasecmp(m_Str.c_str(), lpsz);
	};
	INT CompareNoCase(IN const WCString& str) const
	{
		return (INT)strcasecmp(m_Str.c_str(), str.m_Str.c_str());
	};
	VOID Empty()
	{
		m_Str.clear();
	};
	INT Find(IN const WCString& str) const
	{
		return (INT)m_Str.find(str.m_Str);
	};
	INT Find(IN const CHAR* str) const
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find(str);
	};
	INT Find(IN const CHAR ch) const
	{
		return (INT)m_Str.find(ch);
	};
	INT Find(IN const WCString& str, IN INT nStart) const
	{
		return (INT)m_Str.find(str.m_Str, (size_t)nStart);
	};
	INT Find(IN const CHAR* str, IN INT nStart) const
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find(str, (size_t)nStart);
	};
	INT Find(IN const CHAR ch, IN INT nStart) const
	{
		return (INT)m_Str.find(ch, (size_t)nStart);
	};
	INT FindOneOf(IN const WCString& str) const
	{
		return (INT)m_Str.find_first_of(str.m_Str);
	};
	INT FindOneOf(IN const CHAR* str) const
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find_first_of(str);
	};
	INT FindOneOf(IN const WCString& str, IN INT nStart)
	{
		return (INT)m_Str.find_first_of(str.m_Str, (size_t)nStart);
	};
	INT FindOneOf(IN const CHAR* str, IN INT nStart)
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find_first_of(str, (size_t)nStart);
	};
	INT FindNoCase(IN const WCString& str) const
	{
		WCString sbuf1, sbuf2;
		sbuf1 = str;
		sbuf2 = m_Str.c_str();
		sbuf1.MakeUpper();
		sbuf2.MakeUpper();
		return sbuf2.Find(sbuf1);
	};
	INT FindNoCase(IN const CHAR* str) const
	{
		WCString sbuf1, sbuf2;
		sbuf1 = str;
		sbuf2 = m_Str.c_str();
		sbuf1.MakeUpper();
		sbuf2.MakeUpper();
		return sbuf2.Find(sbuf1);
	};
	INT FindNoCase(IN const CHAR ch) const
	{
		WCString sbuf1, sbuf2;
		sbuf1 = ch;
		sbuf2 = m_Str.c_str();
		sbuf1.MakeUpper();
		sbuf2.MakeUpper();
		return sbuf2.Find(sbuf1);
	};
	VOID AppendFormat(IN const CHAR* lpszFormat, ... )
	{
		WCString sbuf;

		va_list args; 
		va_start(args, lpszFormat);
	
		INT length = 512; 
		INT result = format_arg_list(sbuf.m_Str, length, lpszFormat, args); 
		va_end(args); 
		if(result >= 512) 
		{
			va_start(args, lpszFormat); 
			format_arg_list(sbuf.m_Str, result+1, lpszFormat, args); 
			va_end(args); 
		}
		m_Str += sbuf.m_Str;
		sbuf.Empty();
	};
	VOID Format(IN const CHAR* lpszFormat, ... )
	{
		va_list args; 
		va_start(args, lpszFormat);
	
		INT length = 512; 
		INT result = format_arg_list(m_Str, length, lpszFormat, args); 
		va_end(args); 
		if(result >= 512) 
		{
			va_start(args, lpszFormat); 
			format_arg_list(m_Str, result+1, lpszFormat, args); 
			va_end(args); 
		} 
	};
	VOID FreeExtra()
	{
		m_Str.clear();
	};
	CHAR& GetAt(IN INT nIndex) 
	{
		return m_Str.at((size_t)nIndex);
	};
	const CHAR& GetAt(IN INT nIndex) const
	{
		return m_Str.at((size_t)nIndex);
	};
	// under construction
	const CHAR* GetBuffer(IN INT nMinBufLength)
	{
		m_Str.resize(nMinBufLength);
		return m_Str.c_str();
	};
	const CHAR* GetBuffer()
	{
		return m_Str.c_str();
	}
	INT GetLength() const
	{
		return (INT)m_Str.length();
	};
	SWC_BOOL IsEmpty() const
	{
		return m_Str.empty();
	};
	WCString Left(IN INT nCount) const
	{
		WCString sret;
		INT i = 0, nlen = m_Str.length();;

		if(nCount <= nlen)
		{
			for(i = 0; i < nCount; i++)
			{
				sret += m_Str.at(i);
			}
		}
		else
		{
			for(i = 0; i < nlen; i++)
			{
				sret += m_Str.at(i);
			}
		}
		return sret;
	};
	VOID MakeLower()
	{
		std::transform(m_Str.begin(), m_Str.end(), m_Str.begin(), std::ptr_fun<int, int>(std::tolower));
	};
	// under construction
	VOID MakeReverse()
	{
		STR_ITERATOR curit;

		for(curit = m_Str.begin(); curit < m_Str.end(); curit++)
		{
			CHAR ch = *curit;

			if(0 == isupper(ch)) *curit = toupper(*curit);
			else                 *curit = tolower(*curit);
		}
	};
	VOID MakeUpper()
	{
		std::transform(m_Str.begin(), m_Str.end(), m_Str.begin(), std::ptr_fun<int, int>(std::toupper));
	};
	WCString Mid(IN INT nFirst) const
	{
		WCString sret;
		INT i = 0, nlen = m_Str.length();

		for(i = nFirst; i < nlen; i++)
		{
			sret += m_Str.at(i);
		}
		return sret;
	};
	WCString Mid(IN INT nFirst, IN INT nCount) const
	{
		WCString sret;
		INT i = 0, nlen = m_Str.length();

		if((nFirst + nCount) <= nlen)
		{
			for(i = nFirst; i < (nFirst + nCount); i++)
			{
				sret += m_Str.at(i);
			}
		}
		else
		{
			for(i = nFirst; i < nlen; i++)
			{
				sret += m_Str.at(i);
			}
		}
		return sret;
	};
	// under construction
	VOID ReleaseBuffer()
	{
		m_Str.clear();
	};
	INT Remove(IN CHAR ch)
	{
		SWC_BOOL bfind = FALSE;
		INT nret = 0;

		do
		{
			size_t nfind = 0;
			if(m_Str.npos != (nfind = m_Str.find(ch)))
			{
				bfind = TRUE;
				m_Str.erase(nfind, 1);
				nret++;
			}
			else bfind = FALSE;
		} while (TRUE == bfind);
		return nret;
	};
	INT Replace(IN CHAR chOld, IN CHAR chNew)
	{
		SWC_BOOL bfind = FALSE;
		INT nret = 0;

		do
		{
			size_t nfind = 0;
			if(m_Str.npos != (nfind = m_Str.find(chOld)))
			{
				bfind = TRUE;
				m_Str.replace(nfind, 1, 1, chNew);
				nret++;
			}
			else bfind = FALSE;
		} while (TRUE == bfind);
		return nret;
	};
	INT Replace(IN const CHAR* lpszOld, IN const CHAR* lpszNew)
	{
		SWC_BOOL bfind = FALSE;
		INT nret = 0;
		size_t nlen_old = 0, nlen_new = 0, npos = 0;

		if((NULL == lpszOld) || (NULL == lpszNew)) return 0;
		if( (0 == (nlen_old = strlen(lpszOld))) || 
			(0 == (nlen_new = strlen(lpszNew)))) 
			return 0;
		do
		{
			size_t nfind = 0;
			if(m_Str.npos != (nfind = m_Str.find(lpszOld, npos)))
			{
				bfind = TRUE;
				npos = nfind + nlen_old + 1;
				m_Str.replace(nfind, nlen_old, lpszNew);
				nret += nlen_old;
			}
			else bfind = FALSE;
		} while (TRUE == bfind);
		return nret;
	};
	INT Replace(IN WCString& sOld, IN WCString& sNew)
	{
		return Replace(sOld.m_Str.c_str(), sNew.m_Str.c_str());
	}
	INT Replace(IN CHAR chOld, IN CHAR chNew, IN INT nrepeat)
	{
		SWC_BOOL bfind = FALSE;
		INT nret = 0;
		size_t nfind = 0, npos = 0;

		do
		{
			nfind = m_Str.find(chOld, npos);
			npos = nfind + nrepeat + 1;
			if(m_Str.npos != nfind)
			{
				bfind = TRUE;
				m_Str.replace(nfind, 1, nrepeat, chNew);
				nret++;
			}
			else bfind = FALSE;
		} while (TRUE == bfind);
		return nret;
	}
	INT ReverseFind(IN const CHAR ch) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;
		
		if(m_Str.npos == (nfind = m_Str.rfind(ch))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN const CHAR* str) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(NULL == str) return -1;
		if(m_Str.npos == (nfind = m_Str.rfind(str))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN const WCString& str) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(m_Str.npos == (nfind = m_Str.rfind(str.m_Str))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN CHAR ch, IN INT nStart) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(m_Str.npos == (nfind = m_Str.rfind(ch, (size_t)nStart))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN const CHAR* str, IN INT nStart) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(m_Str.npos == (nfind = m_Str.rfind(str, (size_t)nStart))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN const WCString& str, IN INT nStart) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(m_Str.npos == (nfind = m_Str.rfind(str.m_Str, (size_t)nStart))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	WCString Right(IN INT nCount) const
	{
		WCString sret;
		size_t nlen = m_Str.length();

		if(nlen <= (size_t)nCount) sret = m_Str;
		else               sret = m_Str.substr(nlen - (size_t)nCount);
		return sret;
	};
	VOID SetAt(IN INT nIndex, IN CHAR ch)
	{
		m_Str.replace(nIndex, 1, 1, ch);
	};
	WCString SpanExcluding(IN CHAR ch) const
	{
		WCString sret;
		SWC_BOOL bfind = TRUE;
		
		sret = m_Str;
		do
		{
			size_t nfind = 0;
			if(sret.m_Str.npos != (nfind = sret.m_Str.find(ch)))
			{
				bfind = TRUE;
				sret.m_Str.erase(nfind, 1);
			}
			else bfind = FALSE;
		} while (TRUE == bfind);
		return sret;
	}
	WCString SpanExcluding(IN const CHAR* lpszCharSet) const
	{
		size_t nlen = 0;
		WCString sret;
		SWC_BOOL bfind = TRUE;
		
		sret = m_Str;
		if(NULL == lpszCharSet) return sret;
		if(0 == (nlen = strlen(lpszCharSet))) return sret;
		do
		{
			size_t nfind = 0;
			if(sret.m_Str.npos != (nfind = sret.m_Str.find(lpszCharSet)))
			{
				bfind = TRUE;
				sret.m_Str.erase(nfind, nlen);
			}
			else bfind = FALSE;
		} while (TRUE == bfind);
		return sret;
	};
	WCString SpanExcluding(IN WCString strex) const
	{
		return SpanExcluding(strex.m_Str.c_str());
	}
	VOID TrimLeft()
	{
		size_t nfind = 0;

		if(TRUE == m_Str.empty()) return;
		nfind = m_Str.find_first_not_of(' ');
		if(nfind != m_Str.npos)
		{
			m_Str.erase(0, nfind);
		}
		else
		{
			m_Str.erase(m_Str.begin(), m_Str.end());
		}
	};
	VOID TrimRight()
	{
		size_t nfind = 0;

		if(TRUE == m_Str.empty()) return;
		nfind = m_Str.find_last_not_of(' ');
		if(nfind != m_Str.npos)
		{
			m_Str.erase(nfind+1);
		}
		else
		{
			m_Str.erase(m_Str.begin(), m_Str.end());
		}
	};
	VOID Trim()
	{
		Replace('\r', ' ');
		Replace('\n', ' ');
		TrimLeft();
		TrimRight();
	};
	const WCString& operator =(IN const WCString& str)
	{
		m_Str = str.m_Str;
		return *this;
	};
	const WCString& operator =(IN const _STR_ str)
	{
		m_Str = str;
		return *this;
	};
	const WCString& operator =(IN const UCHAR* psz)
	{
		m_Str = (CHAR*)psz;
		return *this;
	};
	const WCString& operator =(IN const CHAR* lpsz)
	{
		if(NULL == lpsz) return *this;
		m_Str = lpsz;
		return *this;
	};
	const WCString& operator =(IN CHAR ch)
	{
		m_Str = ch;
		return *this;
	};
	const WCString& operator +(IN const WCString& string)
	{
		m_Str += string.m_Str;
		return *this;
	}
	const WCString& operator +(IN const CHAR* string)
	{
		if(NULL == string) return *this;
		m_Str += string;
		return *this;
	}
	const WCString& operator +(IN CHAR ch)
	{
		m_Str += ch;
		return *this;
	}
	const WCString& operator +=(IN const WCString& string)
	{
		m_Str += string.m_Str;
		return *this;
	};
	const WCString& operator +=(IN CHAR ch)
	{
		m_Str += ch;
		return *this;
	};
	const WCString& operator +=(IN const CHAR* lpsz)
	{
		if(NULL == lpsz) return *this;
		m_Str += lpsz;
		return *this;
	};
	const WCString& operator -=(IN CHAR ch)
	{
		m_Str = SpanExcluding(ch).m_Str.c_str();
		return *this;
	}
	const WCString& operator -=(IN const CHAR* lpsz)
	{
		m_Str = SpanExcluding(lpsz).m_Str.c_str();
		return *this;
	}
	const WCString& operator -=(IN const WCString& string)
	{
		m_Str = SpanExcluding(string).m_Str.c_str();
		return *this;
	}
	SWC_BOOL operator ==(IN const WCString& str)
	{
		SWC_BOOL bret = TRUE;

		if(0 != m_Str.compare(str.m_Str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator ==(IN const CHAR* str)
	{
		SWC_BOOL bret = TRUE;

		if(NULL == str) return FALSE;
		if(0 != m_Str.compare(str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator !=(IN const WCString& str)
	{
		SWC_BOOL bret = TRUE;

		if(0 == m_Str.compare(str.m_Str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator !=(IN const CHAR* str)
	{
		SWC_BOOL bret = TRUE;

		if(NULL == str) return FALSE;
		if(0 == m_Str.compare(str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator < (IN const WCString& str)
	{
		SWC_BOOL bret = FALSE;

		if(0 < m_Str.compare(str.m_Str)) bret = TRUE;
		return bret;
	};
	SWC_BOOL operator < (IN const CHAR* str)
	{
		SWC_BOOL bret = FALSE;

		if(NULL == str) return FALSE;
		if(0  < m_Str.compare(str)) bret = TRUE;
		return bret;
	};
	SWC_BOOL operator > (IN const WCString& str)
	{
		SWC_BOOL bret = FALSE;

		if(0 > m_Str.compare(str.m_Str)) bret = TRUE;
		return bret;
	};
	SWC_BOOL operator > (IN const CHAR* str)
	{
		SWC_BOOL bret = FALSE;

		if(NULL == str) return FALSE;
		if(0 > m_Str.compare(str)) bret = TRUE;
		return bret;
	};
	CHAR operator [](IN INT nIndex) const
	{
		return m_Str.at((size_t)nIndex);		
	};
	WCString& Insert(IN INT nIndex, IN const WCString& str)
	{
		m_Str.insert((size_t)nIndex, str.m_Str);
		return *this;
	};
	WCString& Insert(IN INT nIndex, IN const WCString& str, IN INT nIndex2, IN INT nCount)
	{
		m_Str.insert((size_t)nIndex, str.m_Str, (size_t)nIndex2, (size_t)nCount);
		return *this;
	};
	WCString& Insert(IN INT nIndex, IN const CHAR* str, IN INT nCount)
	{
		if(NULL == str) return *this;
		if(0 == strlen(str)) return *this;
		if(0 == nCount) return *this;
		m_Str.insert((size_t)nIndex, str, (size_t)nCount);
		return *this;
	};
	WCString& Insert(IN INT nIndex, IN const CHAR* str)
	{
		if(NULL == str) return *this;
		if(0 == strlen(str)) return *this;
		m_Str.insert((size_t)nIndex, str);
		return *this;
	};
	WCString& Insert(IN INT nIndex, IN CHAR ch)
	{
		m_Str.insert((size_t)nIndex, 1, ch);
		return *this;
	};
	std::wstring ToWStr(IN const CHAR* sFrom, IN const CHAR* sTo)
	{
		std::wstring sret = L"";
		iconv_t cd = 0;
		CHAR *in = NULL, *out = NULL;
		CHAR *pin = NULL, *pout = NULL;
		CHAR fbuf[40] = {0}, tbuf[40] = {0};
		size_t inl = 0, outl = 0;
		INT nret = 0, nWCharSize = (INT)sizeof(wchar_t);
		
		if((NULL == sFrom) || (NULL == sTo)) return sret;
		if((0 == strlen(sFrom)) || (0 == strlen(sTo))) return sret;
		snprintf(fbuf, 40, "%s//IGNORE", sFrom);
		snprintf(tbuf, 40, "%s", sTo);
		//snprintf(tbuf, 40, "UTF-16LE");
		if(0 == strcmp(fbuf, tbuf)) return sret;
		{
			inl = m_Str.length() + 2;
			in = new CHAR[inl];
			outl = (inl * nWCharSize);
			out = new CHAR[outl];
			pin = in;
			pout = out;
			memset(in, 0x00, inl);
			memset(out, 0x00, outl);

			snprintf(in, inl, "%s", m_Str.c_str());
			if((iconv_t)0 > (cd = iconv_open(tbuf, fbuf))) 
			{
				delete [] pin;
				delete [] pout;
				return sret;
			}
			if(0 > (nret = iconv(cd, &in, &inl, &out, &outl))) 
			{
				delete [] pin;
				delete [] pout;
				iconv_close(cd);
				return sret;
			}
			iconv_close(cd);
			sret = (wchar_t*)pout;
			if(NULL != pin) { delete [] pin; pin = NULL; }
			if(NULL != pout) { delete [] pout; pout = NULL; }
		}
		return sret;
	}
	WCString ICONV(IN const CHAR* sFrom, IN const CHAR* sTo)
	{
		WCString sret;
		iconv_t cd = 0;
		CHAR *in = NULL, *out = NULL;
		CHAR *pin = NULL, *pout = NULL;
		CHAR fbuf[40] = {0}, tbuf[40] = {0};
		size_t inl = 0, outl = 0;
		INT nret = 0;

		sret = m_Str;
		if((NULL == sFrom) || (NULL == sTo)) return sret;
		if((0 == strlen(sFrom)) || (0 == strlen(sTo))) return sret;
		snprintf(fbuf, 40, "%s//IGNORE", sFrom);
		snprintf(tbuf, 40, "%s", sTo);
		if(0 == strcmp(fbuf, tbuf)) return sret;
		else
		{
			inl = m_Str.length() + 2;
			in = new CHAR[inl];
			outl = inl * 2;
			out = new CHAR[inl * 2];
			pin = in;
			pout = out;
			memset(in, 0x00, inl);
			memset(out, 0x00, inl * 2);

			snprintf(in, inl, "%s", m_Str.c_str());
			if((iconv_t)0 > (cd = iconv_open(tbuf, fbuf))) 
			{
				delete [] pin;
				delete [] pout;
				return sret;
			}
			if(0 > (nret = iconv(cd, &in, &inl, &out, &outl))) 
			{
				delete [] pin;
				delete [] pout;
				iconv_close(cd);
				return sret;
			}
			iconv_close(cd);
			sret = pout;
			if(NULL != pin) { delete [] pin; pin = NULL; }
			if(NULL != pout) { delete [] pout; pout = NULL; }
		}
		return sret;
	};
	WCString()
	{
		
	};
	WCString(_STR_ str)
	{
		m_Str = str;
	};
	WCString(IN const CHAR* str)
	{
		if(NULL != str) m_Str = str;
	};
	WCString(IN const CHAR ch)
	{
		m_Str = ch;
	};
	WCString(const WCString& str)
	{
		m_Str = str.m_Str;
	};
	~WCString()
	{
		m_Str.clear();
	};
private :
	INT format_arg_list(OUT _STR_& out, IN INT length, IN const CHAR *fmt, IN va_list args)
	{
		INT   result = 0; 
		CHAR *buffer = 0; 

		if (!fmt) return -1; 

		buffer = new CHAR [length + 1]; 
		memset(buffer, 0, length + 1); 
		result = vsnprintf(buffer, length, fmt, args); 
		out = buffer; 
		delete [] buffer; 

		return result; 
	};
public :
	_STR_ m_Str;
	STR_ITERATOR m_Iterator;
	STR_ITERATOR m_rIterator;
};

//-----------------------------------------------------------------------------
#endif // __SWC_STRING_H__
