/**
@file FileVersion.h
@brief ������ ���� ���� �� ������ �ð� ������ �����ϴ� Ŭ����
@date 2010/04/20
@version 1.0
@author sdshim
*/

#pragma once

#pragma comment(lib,"version.lib") 

typedef struct tagVersionInformation
{
	CString sFilePath;
	CString	stCompany;
	CString	stCopyRight;
	CString	stDescription;
	CString	stFileVersion;
	CString stMajorVersion;
	CString stMinorVersion;
	CString	stInternalName;
	CString	stOriginalName;
	CString	stProductName;
	CString	stProductVersion;
	CString	stDllVersion;
	CString	stFileOS;
	CString	stFileType;
	CString stLanguageId;
	CString stCharSet;
	CString stPrivateBuild;
	CString stSpecialBuild;
	VS_FIXEDFILEINFO vsFixedFileInfo;

	struct TRANSLATE 
	{
		WORD languageId;
		WORD characterSet;

	} Translation;

}VERSION_INFORMATION, *LPVERSION_INFORMATION;

static const __int64 SECS_BETWEEN_EPOCHS = 11644473600; //11644473600;
static const __int64 SECS_TO_100NS = 10000000; /* 10^7 */


/// @class CFileVersion
/// @brief ������ ���� ���� �� ������ �ð� ������ �����ϴ� Ŭ����
class CFileVersion  
{
public:
	CFileVersion();
	virtual ~CFileVersion();

	/**
	@brief CFileVersion��ü�� �����ϴ� ������ ������� �ʱ�ȭ�ϴ� �Լ�
	@return void
	*/
	void Init();

	/**
	@brief ������ ������ ������� �ʱ�ȭ�ϴ� �Լ�
	@return void
	*/
	void Init(CString strFilePath/**<  ���������� ������ ������ ������ */);

	/**
	@brief ������ �����ð��� ��� �Լ�.
	@return ���ϻ����ð�.
	*/
	time_t GetCreationTime();
	CString GetCreationTimeToString();

	/**
	@brief ������ ������ ���� �ð��� ��� �Լ�.
	@return  ������ ������ ���� �ð�
	*/
	time_t GetLastAccessTime();

	/**
	@brief ������ ������ ���� �ð��� ��� �Լ�.
	@return  ������ ������ ���� �ð�
	*/
	time_t GetLastWriteTime();
	CString GetLastWriteTimeToString();

	/**
	@brief ������ ���������� ��Ʈ�� ���O�� ��� �Լ�.
	@return ���Ϲ��� ��Ʈ��
	*/
	CString GetFileVersion();

	/**
	@brief ������ PrivateBuild ������ ��� �Լ�.
	@return PrivateBuild ����
	*/
	CString GetPrivateBuild();

	/**
	@brief ������ PrivateBuild ������ ��� �Լ�.
	@return InternalName
	*/
	CString GetInternalName();

	/**
	@brief ������ ProductVersion ������ ��� �Լ�.
	@return ProductVersion
	*/
	CString GetProductVersion();

	/**
	@brief ������ Description ������ ��� �Լ�.
	@return Description
	*/
	CString GetDescription();

	/**
	@brief ���Ͽ� �߰��Ǿ� �ִ� ������ �����ϴ� �Լ�.
	@return ��������
	*/
	BOOL GetFixedFileInfo();

	/**
	@brief ���������� ��� �Լ�
	@return ��������
	*/
	BOOL GetDynamicInfo(BYTE *pVersionInfo/**<  �������� ����ü ������ */);

	/**
	@brief ������ �ð� ������ �����ϴ� �Լ�.
	@return ��������
	*/
	BOOL ReadFileTime();

	/**
	@brief ������ Resource ������ �����ϴ� �Լ�.
	@return ��������
	*/
	BOOL ReadResourceInformation();

	/**
	@brief ������ Resource ������ �����ϴ� �Լ�.
	@return ��������
	*/
	BOOL ReadResourceInformation(CString strFilePath/**<  ���������� ������ ������ ������ */);


	VERSION_INFORMATION		m_VersionInfo;		/**<  �������� ����ü */

private:
	FILETIME				m_CreationTime;		/**<  ������ ���� �ð� */
	FILETIME				m_LastAccessTime;	/**<  ������ ������ ���� �ð� */
	FILETIME				m_LastWriteTime;	/**<  ������ ������ ���� �ð� */

	/**
	@brief ���Ͻð��� ���ýð����� ��ȯ�ϴ� �Լ�
	@return ��������
	*/
	BOOL FileTimeToUnixTime(const FILETIME* ft/**<  ���Ͻð� */, time_t* ut/**<  ���ýð� */, BOOL local_time/**<  GMT�� ��ȯ ���� ����Ÿ������ ��ȯ ���� ���� */ );

	/**
	@brief ���Ͻð��� ���ýð����� ��ȯ�ϴ� �Լ�
	@return ���ýð�
	*/
	time_t FileTimeToUnixTime(FILETIME FileTime/**<  ���Ͻð� */);


};
