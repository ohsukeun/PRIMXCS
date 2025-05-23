
//----------------------------------------------------------------
// 파일의 버젼 정보 및 파일의 시간 정보를 수집하는 클래스
//
//----------------------------------------------------------------

#include "stdafx.h"
#include "FileVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileVersion::CFileVersion()
{
	try
	{
		m_CreationTime.dwHighDateTime = 0;		
		m_LastAccessTime.dwHighDateTime = 0;	
		m_LastWriteTime.dwHighDateTime = 0;	

		m_CreationTime.dwLowDateTime = 0;		
		m_LastAccessTime.dwLowDateTime = 0;	
		m_LastWriteTime.dwLowDateTime =0;	
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}


}

CFileVersion::~CFileVersion()
{

}

time_t CFileVersion::FileTimeToUnixTime(FILETIME FileTime)
{
	__int64 UnixTime = 0;
	try
	{
		UnixTime = ((__int64)FileTime.dwHighDateTime << 32) + FileTime.dwLowDateTime;
		UnixTime = UnixTime - (SECS_BETWEEN_EPOCHS * SECS_TO_100NS);
		UnixTime /= SECS_TO_100NS; /* now convert to seconds */
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return (time_t)UnixTime;
}

BOOL CFileVersion::FileTimeToUnixTime(const FILETIME* ft, time_t* ut, BOOL local_time )
{
	BOOL success = FALSE;
	try
	{
		if ( local_time ) 
		{
			struct tm atm;
			SYSTEMTIME st;
			success = FileTimeToSystemTime ( ft, &st );
			atm.tm_sec = st.wSecond;
			atm.tm_min = st.wMinute;
			atm.tm_hour = st.wHour;
			atm.tm_mday = st.wDay;
			/* tm_mon is 0 based */
			atm.tm_mon = st.wMonth - 1;
			/* tm_year is 1900 based */
			atm.tm_year = st.wYear>1900?st.wYear - 1900:st.wYear;     
			atm.tm_isdst = -1;      /* see notes above */
			*ut = mktime ( &atm );
		}
		else 
		{
			/* One second = 10,000,000 * 100 nsec */
			const ULONGLONG second = 10000000L;

			SYSTEMTIME base_st = 
			{
				1970,   /* wYear            */
				1,      /* wMonth           */
				0,      /* wDayOfWeek       */
				1,      /* wDay             */
				0,      /* wHour            */
				0,      /* wMinute          */
				0,      /* wSecond          */
				0       /* wMilliseconds    */
			};

			ULARGE_INTEGER itime;
			FILETIME base_ft;

			success = SystemTimeToFileTime ( &base_st, &base_ft );
			if (success) 
			{
				itime.QuadPart = ((ULARGE_INTEGER *)ft)->QuadPart;

				itime.QuadPart -= ((ULARGE_INTEGER *)&base_ft)->QuadPart;
				itime.QuadPart /= second;

				*ut = itime.LowPart;
			}
		}
		if (!success)
		{
			*ut = -1;   /* error value used by mktime() */
		}
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return success;
}

time_t CFileVersion::GetCreationTime()
{
	return FileTimeToUnixTime(m_CreationTime);
}

CString CFileVersion::GetCreationTimeToString()
{
	CString strDate;
	try
	{
		CTime tFileTime(m_CreationTime);
		strDate = tFileTime.Format(_T("%Y%m%d%H%M%S"));
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return strDate;
}

time_t CFileVersion::GetLastAccessTime()
{
	return FileTimeToUnixTime(m_LastAccessTime);
}

time_t CFileVersion::GetLastWriteTime()
{
	return FileTimeToUnixTime(m_LastWriteTime);
}

CString CFileVersion::GetLastWriteTimeToString()
{
	CString strDate;
	try
	{
		CTime tFileTime(m_LastWriteTime);
		strDate = tFileTime.Format(_T("%Y%m%d%H%M%S"));
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return strDate;
}

BOOL CFileVersion::ReadResourceInformation(CString strFilePath)
{
	BOOL bRe = FALSE;
	try
	{
		DWORD	dwLength,dwNullHandle;
		LPVOID	lpVersionPtr;
		UINT	uiVerLength;

		m_VersionInfo.sFilePath = strFilePath;

		dwLength = ::GetFileVersionInfoSize (strFilePath, &dwNullHandle);
		if (dwLength <= 0) 
			return FALSE;

		BYTE* pVersionInfo = new BYTE [dwLength];
		if (!::GetFileVersionInfo (strFilePath, NULL, dwLength, pVersionInfo))
		{
			return FALSE;
		}
		if (!::VerQueryValue (pVersionInfo, _T ("\\"), &lpVersionPtr, &uiVerLength))
		{
			return FALSE;
		}

		m_VersionInfo.vsFixedFileInfo = *(VS_FIXEDFILEINFO*)lpVersionPtr;

		if (!GetDynamicInfo(pVersionInfo))
		{
			return FALSE;
		}
		if (!GetFixedFileInfo ())
		{
			return FALSE;
		}

		_SafeDelete( pVersionInfo);
		bRe = TRUE;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return bRe;
}
BOOL CFileVersion::ReadResourceInformation()
{
	BOOL bRe = FALSE;

	try
	{
		DWORD	dwLength, dwSize;
		TCHAR	szFileName [MAX_PATH];
	
		dwSize = sizeof (szFileName) / sizeof (szFileName [0]);
		dwLength = ::GetModuleFileName (AfxGetInstanceHandle(), szFileName, dwSize);
		if (dwLength <= 0) 
			return FALSE;

		bRe = ReadResourceInformation(szFileName);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return bRe;
}

BOOL CFileVersion::ReadFileTime()
{
	BOOL bRe = FALSE;
	try
	{
		CFile file;

		if(file.Open(m_VersionInfo.sFilePath,CFile::modeRead,NULL))
		{
			bRe=GetFileTime((HANDLE)file.m_hFile,&m_CreationTime,&m_LastAccessTime,&m_LastWriteTime);
			file.Close();
		}
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return bRe;
}


BOOL CFileVersion::GetDynamicInfo(BYTE *pVersionInfo)
{
	BOOL bRe = FALSE;
	try
	{
		UINT	uiVerLength;
		LPVOID	lpPtr;
		CString	sQuery;

		if (::VerQueryValue (pVersionInfo, _T("\\VarFileInfo\\Translation"), &lpPtr, &uiVerLength))
			m_VersionInfo.Translation = *(VERSION_INFORMATION::TRANSLATE*)lpPtr;
	
		sQuery.Format (	_T ("\\StringFileInfo\\%04x%04x\\CompanyName"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stCompany=CString((LPCTSTR)lpPtr);
	
		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\LegalCopyRight"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stCopyRight=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\ProductName"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stProductName=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\ProductVersion"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stProductVersion=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\OriginalFileName"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stOriginalName=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\FileDescription"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stDescription=CString((LPCTSTR)lpPtr);
	
		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\FileVersion"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stFileVersion=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\InternalName"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stInternalName=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\PrivateBuild"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stPrivateBuild=CString((LPCTSTR)lpPtr);

		sQuery.Format (_T ("\\StringFileInfo\\%04x%04x\\SpecialBuild"),m_VersionInfo.Translation.languageId, m_VersionInfo.Translation.characterSet);
		if(TRUE == ::VerQueryValue (pVersionInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr,&uiVerLength)) m_VersionInfo.stSpecialBuild=CString((LPCTSTR)lpPtr);

		bRe = TRUE;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return bRe;
}


BOOL CFileVersion::GetFixedFileInfo()
{
	BOOL bRe = FALSE;

	try
	{
		m_VersionInfo.stMajorVersion.Format(_T("%d"),HIWORD (m_VersionInfo.vsFixedFileInfo.dwFileVersionMS));
		m_VersionInfo.stMinorVersion.Format(_T("%d"),LOWORD (m_VersionInfo.vsFixedFileInfo.dwFileVersionMS));
		m_VersionInfo.stDllVersion.Format (_T("%d.%d.%d.%d"), 
			HIWORD (m_VersionInfo.vsFixedFileInfo.dwFileVersionMS), LOWORD (m_VersionInfo.vsFixedFileInfo.dwFileVersionMS),
			HIWORD (m_VersionInfo.vsFixedFileInfo.dwFileVersionLS), LOWORD (m_VersionInfo.vsFixedFileInfo.dwFileVersionLS));

		if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_DRV)
		{
			switch (m_VersionInfo.vsFixedFileInfo.dwFileSubtype) 
			{
				case VFT2_DRV_DISPLAY:
					m_VersionInfo.stFileType = _T("Display driver");
					break;
				case VFT2_DRV_INSTALLABLE:
					m_VersionInfo.stFileType = _T("Installable driver");
					break;
				case VFT2_DRV_KEYBOARD:
					m_VersionInfo.stFileType = _T("Keyboard driver");
					break;
				case VFT2_DRV_LANGUAGE:
					m_VersionInfo.stFileType = _T("Language driver");
					break;
				case VFT2_DRV_MOUSE:
					m_VersionInfo.stFileType = _T("Mouse driver");
					break;
				case VFT2_DRV_NETWORK:
					m_VersionInfo.stFileType = _T("Network driver");
					break;
				case VFT2_DRV_PRINTER:
					m_VersionInfo.stFileType = _T("Printer driver");
					break;
				case VFT2_DRV_SOUND:
					m_VersionInfo.stFileType = _T("Sound driver");
					break;
				case VFT2_DRV_SYSTEM:
					m_VersionInfo.stFileType = _T("System driver");
					break;
				case VFT2_UNKNOWN:
					m_VersionInfo.stFileType = _T("Unknown driver");
					break;
			}
		}
		else if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_FONT) 
		{
			switch (m_VersionInfo.vsFixedFileInfo.dwFileSubtype) 
			{
				case VFT2_FONT_RASTER:
					m_VersionInfo.stFileType = _T ("Raster font");
					break;
				case VFT2_FONT_TRUETYPE:
					m_VersionInfo.stFileType = _T ("Truetype font");
					break;
				case VFT2_FONT_VECTOR:
					m_VersionInfo.stFileType = _T ("Vector font");
					break;
				case VFT2_UNKNOWN:
					m_VersionInfo.stFileType = _T ("Unknown font");
					break;
			}
		}
		else if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_APP) 
		{
			m_VersionInfo.stFileType = _T ("Application");
		}
		else if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_DLL) 
		{
			m_VersionInfo.stFileType = _T ("Dynamic link library");
		}
		else if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_STATIC_LIB) 
		{
			m_VersionInfo.stFileType = _T ("Static link library");
		}
		else if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_VXD) 
		{
			m_VersionInfo.stFileType = _T ("Virtual device");
		}
		else if (m_VersionInfo.vsFixedFileInfo.dwFileType == VFT_UNKNOWN) 
		{
			m_VersionInfo.stFileType = _T ("Unknown type");
		}

		switch (m_VersionInfo.vsFixedFileInfo.dwFileOS) 
		{
			case VOS_DOS:
				m_VersionInfo.stFileOS = _T ("MS-DOS");
				break;
			case VOS_DOS_WINDOWS16:
				m_VersionInfo.stFileOS = _T ("16-bit windows running on MS-DOS");
				break;
			case VOS_DOS_WINDOWS32:
				m_VersionInfo.stFileOS = _T ("Win32 API running on MS-DOS");
				break;
			case VOS_OS216:
				m_VersionInfo.stFileOS = _T ("16-bit OS/2");
				break;
			case VOS_OS216_PM16:
				m_VersionInfo.stFileOS = _T ("16-bit Presentation manager running on 16-bit OS/2");
				break;
			case VOS_OS232:
				m_VersionInfo.stFileOS = _T ("32-bit OS/2");
				break;
			case VOS_NT:
				m_VersionInfo.stFileOS = _T ("Windows NT");
				break;
			case VOS_NT_WINDOWS32:
				m_VersionInfo.stFileOS = _T ("Win32 API on Windows NT");
				break;
			case VOS_UNKNOWN:
				m_VersionInfo.stFileOS = _T ("Unknown OS");
				break;
		}

		bRe = TRUE;
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return bRe;
}
CString CFileVersion::GetInternalName()
{
	return m_VersionInfo.stInternalName;
}
CString CFileVersion::GetProductVersion()
{
	return m_VersionInfo.stProductVersion;
}

CString CFileVersion::GetPrivateBuild()
{
	return m_VersionInfo.stPrivateBuild;
}
CString CFileVersion::GetDescription()
{
	return m_VersionInfo.stDescription;
}
void CFileVersion::Init()
{
	try
	{
		ReadResourceInformation();
		ReadFileTime();
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}
}
void CFileVersion::Init(CString strFilePath)
{
	try
	{
		ReadResourceInformation(strFilePath);
		ReadFileTime();
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}
}

CString CFileVersion::GetFileVersion()
{
	CString stFileVersion;

	try
	{
		stFileVersion = m_VersionInfo.stFileVersion;
		stFileVersion.Replace(_T(" "),_T(""));
		stFileVersion.Replace(_T(","),_T("."));
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(...)
	{
	}

	return stFileVersion;
}
