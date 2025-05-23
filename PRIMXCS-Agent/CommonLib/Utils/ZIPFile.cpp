#include "stdafx.h"
#include "ZIPFile.h"

#include "XUnzip.h"
#include "Xzip.h"


CZIPFile::CZIPFile()
{
}


CZIPFile::~CZIPFile()
{
}

BOOL CZIPFile::DoZipFile(CString strTargetDir, CString strZipDir)
{
	if (GetFileAttributes(strTargetDir) == 0xFFFFFFFF)
		return FALSE;

	CStringArray strArray, strErrorArray;
	HZIP hz = CreateZip(strZipDir.GetBuffer(0), 0, ZIP_FILENAME); // 관리자 권한이 필요한 경로면 실패할 수 있음. 
	if (hz)
	{
#ifdef _UNICODE
		ZIPENTRYW ze;
#else
		ZIPENTRY ze;
#endif
		memset(&ze, 0, sizeof(ze));

		CFileFind	ff;
		CString		sSearchFile;
		CString		sTargetFile, sTemp;

		strArray.Add(strTargetDir);
		while (strArray.GetCount() > 0)
		{
			sSearchFile.Format(_T("%s\\*.*"), strArray.GetAt(0));
			BOOL res = ff.FindFile(sSearchFile);
			while (res)
			{
				res = ff.FindNextFile();
				if (!ff.IsDots() && ff.IsDirectory())
				{
					strArray.Add(ff.GetFilePath());
					sTargetFile = ff.GetFilePath();

					sTemp.Format(_T("%s\\"), strTargetDir);
					sTargetFile.Replace(sTemp, _T(""));

					if (ZipAdd(hz, sTargetFile, 0, 0, ZIP_FOLDER) != ZR_OK)
						strErrorArray.Add(ff.GetFilePath());  // 폴더 추가 오류
				}
				else if (!ff.IsDots())
				{
					sTargetFile = ff.GetFilePath();
					if (strZipDir == sTargetFile)  // 자기 자신은 제외
						continue;

					sTemp.Format(_T("%s\\"), strTargetDir);
					sTargetFile.Replace(sTemp, _T(""));

					if (ZipAdd(hz, sTargetFile, (ff.GetFilePath().GetBuffer(0)), 0, ZIP_FILENAME) != ZR_OK)
						strErrorArray.Add(ff.GetFilePath());  // 파일 추가 오류
				}
			}
			ff.Close();
			strArray.RemoveAt(0);
		}

		CloseZip(hz);
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CZIPFile::DoUnzipFile(CString strTargetDir, CString strUnzipDir, CStringList& UnZipFileList)
{
	if (GetFileAttributes(strTargetDir) == 0xFFFFFFFF)
		return FALSE;

	if (GetFileAttributes(strUnzipDir) == 0xFFFFFFFF)
		CreateDirectory(strUnzipDir, NULL);

	ZRESULT zr = 0;
	TCHAR szCurrentdir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentdir);
	SetCurrentDirectory(strUnzipDir);

	HZIP hz = OpenZip((TCHAR*)(LPCTSTR)strTargetDir, 0, ZIP_FILENAME);
	if (hz)
	{
#ifdef _UNICODE
		ZIPENTRYW ze;
#else
		ZIPENTRY ze;
#endif
		ZeroMemory(&ze, sizeof(ze));
		GetZipItem(hz, -1, &ze);

		int numitems = ze.index;
		for (int i = 0; i < numitems; i++)
		{
			zr = GetZipItem(hz, i, &ze);
			if (zr == ZR_OK)
			{
				zr = UnzipItem(hz, i, ze.name, 0, ZIP_FILENAME);
				if ((ze.attr & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					CString strFileName = ze.name;
					UnZipFileList.AddTail(strFileName);
				}
			}
		}
		CloseZip(hz);
	}

	SetCurrentDirectory(szCurrentdir);
	return TRUE;
}