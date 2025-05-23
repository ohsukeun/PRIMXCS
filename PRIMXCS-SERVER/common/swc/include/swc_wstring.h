/*****************************************************************************
 *                                                                           *
 * File Name : swc_wstring.h                                                 *
 * Theme     : wstring wrapper class                                         *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '18. 03. 22                     *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <algorithm>
#include <cctype>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iconv.h>

#include <swc_base.h>
//#include <swc_safty.h>

#ifndef __SWC_WSTRING_H__
#define __SWC_WSTRING_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#define WSTR_ITERATOR  std::wstring::iterator
#define WSTR_RITERATOR std::wstring::reverse_iterator
#define _WSTR_         std::wstring

// related macro definitions
#define LPCWSTR(X) (const wchar_t*)X.m_Str.c_str()
#define LPWSTR(X)  (wchar_t*)X.m_Str.c_str()

/// string class (std::wstring wrapper)
class WCWString
{
public :
	const WCWString& Append(IN const WCWString& string)
	{
		m_Str += string.m_Str;
		return *this;
	};
	const WCWString& Append(IN wchar_t ch)
	{
		m_Str += ch;
		return *this;
	};
	const WCWString& Append(IN const wchar_t* lpsz)
	{
		if(NULL == lpsz) return *this;
		m_Str += lpsz;
		return *this;
	};
	INT Compare(IN const wchar_t* lpsz) const
	{
		if(NULL == lpsz) return 0;
		return m_Str.compare(lpsz);
	};
	INT Compare(IN _WSTR_ str) const
	{
		return m_Str.compare(str);
	};
	INT Compare(IN const WCWString& str) const
	{
		return m_Str.compare(str.m_Str);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const WCWString& str) const
	{
		return m_Str.compare((size_t)nIndex, (size_t)nCount, str.m_Str);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const wchar_t* str) const
	{
		if(NULL == str) return 0;
		return m_Str.compare((size_t)nIndex, nCount, str);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const WCWString& str, IN INT nIndex2, IN INT nCount2) const
	{
		return m_Str.compare((size_t)nIndex, (size_t)nCount, str.m_Str, (size_t)nIndex2, (size_t)nCount2);
	};
	INT Compare(IN INT nIndex, IN INT nCount, IN const wchar_t* str, IN INT nCount2) const
	{
		if(NULL == str) return 0;
		return m_Str.compare((size_t)nIndex, (size_t)nCount, str, (size_t)nCount2);
	};
	INT CompareNoCase(IN const wchar_t* lpsz) const
	{
		if(NULL == lpsz) return 1;
		return (INT)wcscmp(m_Str.c_str(), lpsz);
	};
	INT CompareNoCase(IN const WCWString& str) const
	{
		return (INT)wcscmp(m_Str.c_str(), str.m_Str.c_str());
	};
	VOID Empty()
	{
		m_Str.clear();
	};
	INT Find(IN const WCWString& str) const
	{
		return (INT)m_Str.find(str.m_Str);
	};
	INT Find(IN const wchar_t* str) const
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find(str);
	};
	INT Find(IN const wchar_t ch) const
	{
		return (INT)m_Str.find(ch);
	};
	INT Find(IN const WCWString& str, IN INT nStart) const
	{
		return (INT)m_Str.find(str.m_Str, (size_t)nStart);
	};
	INT Find(IN const wchar_t* str, IN INT nStart) const
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find(str, (size_t)nStart);
	};
	INT Find(IN const wchar_t ch, IN INT nStart) const
	{
		return (INT)m_Str.find(ch, (size_t)nStart);
	};
	INT FindOneOf(IN const WCWString& str) const
	{
		return (INT)m_Str.find_first_of(str.m_Str);
	};
	INT FindOneOf(IN const wchar_t* str) const
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find_first_of(str);
	};
	INT FindOneOf(IN const WCWString& str, IN INT nStart)
	{
		return (INT)m_Str.find_first_of(str.m_Str, (size_t)nStart);
	};
	INT FindOneOf(IN const wchar_t* str, IN INT nStart)
	{
		if(NULL == str) return -1;
		return (INT)m_Str.find_first_of(str, (size_t)nStart);
	};
	VOID AppendFormat(IN const wchar_t* lpszFormat, ... )
	{
		WCWString sbuf;

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
	}
	VOID Format(IN const wchar_t* lpszFormat, ... )
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
	wchar_t& GetAt(IN INT nIndex) 
	{
		return m_Str.at((size_t)nIndex);
	};
	const wchar_t& GetAt(IN INT nIndex) const
	{
		return m_Str.at((size_t)nIndex);
	};
	// under construction
	const wchar_t* GetBuffer(IN INT nMinBufLength)
	{
		m_Str.resize(nMinBufLength);
		return m_Str.c_str();
	};
	const wchar_t* GetBuffer()
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
	WCWString Left(IN INT nCount) const
	{
		WCWString sret;
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
		WSTR_ITERATOR curit;

		for(curit = m_Str.begin(); curit < m_Str.end(); curit++)
		{
			wchar_t ch = *curit;

			if(0 == isupper(ch)) *curit = toupper(*curit);
			else                 *curit = tolower(*curit);
		}
	};
	VOID MakeUpper()
	{
		std::transform(m_Str.begin(), m_Str.end(), m_Str.begin(), std::ptr_fun<int, int>(std::toupper));
	};
	WCWString Mid(IN INT nFirst) const
	{
		WCWString sret;
		INT i = 0, nlen = m_Str.length();

		for(i = nFirst; i < nlen; i++)
		{
			sret += m_Str.at(i);
		}
		return sret;
	};
	WCWString Mid(IN INT nFirst, IN INT nCount) const
	{
		WCWString sret;
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
	INT Remove(IN wchar_t ch)
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
	INT Replace(IN wchar_t chOld, IN wchar_t chNew)
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
	INT Replace(IN const wchar_t* lpszOld, IN const wchar_t* lpszNew)
	{
		SWC_BOOL bfind = FALSE;
		INT nret = 0;
		size_t nlen_old = 0, nlen_new = 0, npos = 0;

		if((NULL == lpszOld) || (NULL == lpszNew)) return 0;
		if( (0 == (nlen_old = wcslen(lpszOld))) || 
			(0 == (nlen_new = wcslen(lpszNew)))) 
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
	INT Replace(IN WCWString& sOld, IN WCWString& sNew)
	{
		return Replace(sOld.m_Str.c_str(), sNew.m_Str.c_str());
	}
	INT Replace(IN wchar_t chOld, IN wchar_t chNew, IN INT nrepeat)
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
	INT ReverseFind(IN wchar_t ch) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;
		
		if(m_Str.npos == (nfind = m_Str.rfind(ch))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN const wchar_t* str) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(NULL == str) return -1;
		if(m_Str.npos == (nfind = m_Str.rfind(str))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	INT ReverseFind(IN const WCWString& str) const
	{
		INT nret = -1;
		size_t nfind = m_Str.npos;

		if(m_Str.npos == (nfind = m_Str.rfind(str.m_Str))) nret = -1;
		else nret = (INT)nfind;
		return nret;
	};
	WCWString Right(IN INT nCount) const
	{
		WCWString sret;
		size_t nlen = m_Str.length();

		if(nlen <= (size_t)nCount) sret = m_Str;
		else               sret = m_Str.substr(nlen - (size_t)nCount);
		return sret;
	};
	VOID SetAt(IN INT nIndex, IN wchar_t ch)
	{
		m_Str.insert(nIndex, 1, ch);
	};
	WCWString SpanExcluding(IN wchar_t ch) const
	{
		WCWString sret;
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
	WCWString SpanExcluding(IN const wchar_t* lpszCharSet) const
	{
		size_t nlen = 0;
		WCWString sret;
		SWC_BOOL bfind = TRUE;
		
		sret = m_Str;
		if(NULL == lpszCharSet) return sret;
		if(0 == (nlen = wcslen(lpszCharSet))) return sret;
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
	WCWString SpanExcluding(IN WCWString strex) const
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
	const WCWString& operator =(IN const CHAR* str)
	{
		_STR_ sStr(str);
		_WSTR_ sWStr(sStr.begin(), sStr.end());
		m_Str = sWStr;
		return *this;
	}
	const WCWString& operator =(IN const WCWString& str)
	{
		m_Str = str.m_Str;
		return *this;
	};
	const WCWString& operator =(IN const _WSTR_ str)
	{
		m_Str = str;
		return *this;
	};
	const WCWString& operator =(IN const wchar_t* lpsz)
	{
		if(NULL == lpsz) return *this;
		m_Str = lpsz;
		return *this;
	};
	const WCWString& operator =(IN wchar_t ch)
	{
		m_Str = ch;
		return *this;
	};
	const WCWString& operator +(IN const WCWString& string)
	{
		m_Str += string.m_Str;
		return *this;
	}
	const WCWString& operator +(IN const wchar_t* string)
	{
		if(NULL == string) return *this;
		m_Str += string;
		return *this;
	}
	const WCWString& operator +(IN wchar_t ch)
	{
		m_Str += ch;
		return *this;
	}
	const WCWString& operator +=(IN const WCWString& string)
	{
		m_Str += string.m_Str;
		return *this;
	};
	const WCWString& operator +=(IN wchar_t ch)
	{
		m_Str += ch;
		return *this;
	};
	const WCWString& operator +=(IN const wchar_t* lpsz)
	{
		if(NULL == lpsz) return *this;
		m_Str += lpsz;
		return *this;
	};
	const WCWString& operator -=(IN wchar_t ch)
	{
		m_Str = SpanExcluding(ch).m_Str.c_str();
		return *this;
	}
	const WCWString& operator -=(IN const wchar_t* lpsz)
	{
		m_Str = SpanExcluding(lpsz).m_Str.c_str();
		return *this;
	}
	const WCWString& operator -=(IN const WCWString& string)
	{
		m_Str = SpanExcluding(string).m_Str.c_str();
		return *this;
	}
	SWC_BOOL operator ==(IN const WCWString& str)
	{
		SWC_BOOL bret = TRUE;

		if(0 != m_Str.compare(str.m_Str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator == (IN const wchar_t* str)
	{
		SWC_BOOL bret = TRUE;

		if(NULL == str) return FALSE;
		if(0 != m_Str.compare(str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator !=(IN const WCWString& str)
	{
		SWC_BOOL bret = TRUE;

		if(0 == m_Str.compare(str.m_Str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator !=(IN const wchar_t* str)
	{
		SWC_BOOL bret = TRUE;

		if(NULL == str) return FALSE;
		if(0 == m_Str.compare(str)) bret = FALSE;
		return bret;
	};
	SWC_BOOL operator < (IN const WCWString& str)
	{
		SWC_BOOL bret = FALSE;

		if(0 < m_Str.compare(str.m_Str)) bret = TRUE;
		return bret;
	};
	SWC_BOOL operator < (IN const wchar_t* str)
	{
		SWC_BOOL bret = FALSE;

		if(NULL == str) return FALSE;
		if(0  < m_Str.compare(str)) bret = TRUE;
		return bret;
	};
	SWC_BOOL operator > (IN const WCWString& str)
	{
		SWC_BOOL bret = FALSE;

		if(0 > m_Str.compare(str.m_Str)) bret = TRUE;
		return bret;
	};
	SWC_BOOL operator > (IN const wchar_t* str)
	{
		SWC_BOOL bret = FALSE;

		if(NULL == str) return FALSE;
		if(0 > m_Str.compare(str)) bret = TRUE;
		return bret;
	};
	wchar_t operator [](IN INT nIndex) const
	{
		return m_Str.at((size_t)nIndex);		
	};
	WCWString& Insert(IN INT nIndex, IN const WCWString& str)
	{
		m_Str.insert((size_t)nIndex, str.m_Str);
		return *this;
	};
	WCWString& Insert(IN INT nIndex, IN const WCWString& str, IN INT nIndex2, IN INT nCount)
	{
		m_Str.insert((size_t)nIndex, str.m_Str, (size_t)nIndex2, (size_t)nCount);
		return *this;
	};
	WCWString& Insert(IN INT nIndex, IN const wchar_t* str, IN INT nCount)
	{
		if(NULL == str) return *this;
		if(0 == wcslen(str)) return *this;
		if(0 == nCount) return *this;
		m_Str.insert((size_t)nIndex, str, (size_t)nCount);
		return *this;
	};
	WCWString& Insert(IN INT nIndex, IN const wchar_t* str)
	{
		if(NULL == str) return *this;
		if(0 == wcslen(str)) return *this;
		m_Str.insert((size_t)nIndex, str);
		return *this;
	};
	WCWString& Insert(IN INT nIndex, IN wchar_t ch)
	{
		m_Str.insert((size_t)nIndex, 1, ch);
		return *this;
	};
	WCString ToMBTS()
	{
		WCString sret = "";
		sret.m_Str.assign(m_Str.begin(), m_Str.end());
		return sret;
	}
	WCString ToUTF8(IN const CHAR* sFrom)
	{
		WCString sret = "";
		iconv_t cd = 0;
		CHAR *in = NULL, *out = NULL;
		CHAR *pin = NULL, *pout = NULL;
		CHAR fbuf[40] = {0}, tbuf[40] = {0};
		size_t inl = 0, outl = 0;
		INT nret = 0, nWCharSize = (INT)sizeof(wchar_t);

		if(NULL == sFrom) return sret;
		if(0 == strlen(sFrom)) return sret;
		snprintf(fbuf, 40, "%s//IGNORE", sFrom);
		snprintf(tbuf, 40, "UTF-8");
		if(0 == strcmp(fbuf, tbuf)) return sret;
		else
		{
			inl = (m_Str.length() * nWCharSize) + nWCharSize;
			in = new CHAR[inl];
			pin = in;
			outl = inl;
			out = new CHAR[outl];
			pout = out;
			memset(in, 0x00, inl);
			memset(out, 0x00, outl);

			memcpy((PVOID)in, (PVOID)m_Str.c_str(), ((m_Str.length() * nWCharSize) + nWCharSize));

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
	WCWString()
	{
		
	};
	WCWString(IN const CHAR* str)
	{
		std::string  sStr(str);
		std::wstring sWStr(sStr.begin(), sStr.end());
		m_Str = sWStr;
	}
	WCWString(IN const std::string sStr)
	{
		std::wstring sWStr(sStr.begin(), sStr.end());
		m_Str = sWStr;
	}
	WCWString(_WSTR_ str)
	{
		m_Str = str;
	};
	WCWString(IN const wchar_t* str)
	{
		if(NULL != str) m_Str = str;
	};
	WCWString(IN const wchar_t ch)
	{
		m_Str = ch;
	};
	WCWString(const WCWString& str)
	{
		m_Str = str.m_Str;
	};
	~WCWString()
	{
		m_Str.clear();
	};
private :
	INT format_arg_list(OUT _WSTR_& out, IN INT length, IN const wchar_t *fmt, IN va_list args)
	{
		INT   result = 0; 
		wchar_t *buffer = 0; 

		if (!fmt) return -1; 

		buffer = new wchar_t [length + 1]; 
		memset(buffer, 0, length + 1); 
		result = vswprintf(buffer, length, fmt, args); 
		out = buffer; 
		delete [] buffer; 

		return result; 
	};
public :
	_WSTR_ m_Str;
	WSTR_ITERATOR m_Iterator;
	WSTR_ITERATOR m_rIterator;
};

//-----------------------------------------------------------------------------
#endif // __SWC_WSTRING_H__
