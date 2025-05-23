#pragma once


class CZIPFile
{
public:
	CZIPFile();
	~CZIPFile();
	
	BOOL DoZipFile(CString strTargetDir, CString strZipDir);
	BOOL DoUnzipFile(CString strTargetDir, CString strUnzipDir,CStringList& UnZipFileList);

};

