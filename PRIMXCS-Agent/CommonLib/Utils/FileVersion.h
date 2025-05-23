/**
@file FileVersion.h
@brief 파일의 버젼 정보 및 파일의 시간 정보를 수집하는 클래스
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
/// @brief 파일의 버젼 정보 및 파일의 시간 정보를 수집하는 클래스
class CFileVersion  
{
public:
	CFileVersion();
	virtual ~CFileVersion();

	/**
	@brief CFileVersion객체를 실행하는 파일을 대상으로 초기화하는 함수
	@return void
	*/
	void Init();

	/**
	@brief 지정한 파일을 대상으로 초기화하는 함수
	@return void
	*/
	void Init(CString strFilePath/**<  버젼정보를 수집할 파일의 절대경로 */);

	/**
	@brief 파일의 생성시간을 얻는 함수.
	@return 파일생성시간.
	*/
	time_t GetCreationTime();
	CString GetCreationTimeToString();

	/**
	@brief 파일의 마지막 접근 시간을 얻는 함수.
	@return  파일의 마지막 접근 시간
	*/
	time_t GetLastAccessTime();

	/**
	@brief 파일의 마지막 수정 시간을 얻는 함수.
	@return  파일의 마지막 수정 시간
	*/
	time_t GetLastWriteTime();
	CString GetLastWriteTimeToString();

	/**
	@brief 파일의 버젼정보를 스트링 형턔로 얻는 함수.
	@return 파일버젼 스트링
	*/
	CString GetFileVersion();

	/**
	@brief 파일의 PrivateBuild 버젼을 얻는 함수.
	@return PrivateBuild 버젼
	*/
	CString GetPrivateBuild();

	/**
	@brief 파일의 PrivateBuild 버젼을 얻는 함수.
	@return InternalName
	*/
	CString GetInternalName();

	/**
	@brief 파일의 ProductVersion 버젼을 얻는 함수.
	@return ProductVersion
	*/
	CString GetProductVersion();

	/**
	@brief 파일의 Description 버젼을 얻는 함수.
	@return Description
	*/
	CString GetDescription();

	/**
	@brief 파일에 추가되어 있는 정보를 수집하는 함수.
	@return 성공여부
	*/
	BOOL GetFixedFileInfo();

	/**
	@brief 버젼정보를 얻는 함수
	@return 성공여부
	*/
	BOOL GetDynamicInfo(BYTE *pVersionInfo/**<  버젼정보 구조체 퍼인터 */);

	/**
	@brief 파일의 시간 정보를 수집하는 함수.
	@return 성공여부
	*/
	BOOL ReadFileTime();

	/**
	@brief 파일의 Resource 정보를 수집하는 함수.
	@return 성공여부
	*/
	BOOL ReadResourceInformation();

	/**
	@brief 파일의 Resource 정보를 수집하는 함수.
	@return 성공여부
	*/
	BOOL ReadResourceInformation(CString strFilePath/**<  버젼정보를 수집할 파일의 절대경로 */);


	VERSION_INFORMATION		m_VersionInfo;		/**<  정보정보 구조체 */

private:
	FILETIME				m_CreationTime;		/**<  파일의 생성 시간 */
	FILETIME				m_LastAccessTime;	/**<  파일의 마지막 접근 시간 */
	FILETIME				m_LastWriteTime;	/**<  파일의 마지막 수정 시간 */

	/**
	@brief 파일시간을 로컬시간으로 변환하는 함수
	@return 성공여부
	*/
	BOOL FileTimeToUnixTime(const FILETIME* ft/**<  파일시간 */, time_t* ut/**<  로컬시간 */, BOOL local_time/**<  GMT로 변환 할지 로컬타임으로 변환 할지 여부 */ );

	/**
	@brief 파일시간을 로컬시간으로 변환하는 함수
	@return 로컬시간
	*/
	time_t FileTimeToUnixTime(FILETIME FileTime/**<  파일시간 */);


};
