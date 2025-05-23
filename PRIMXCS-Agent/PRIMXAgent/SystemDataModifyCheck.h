#pragma once
#include "ConfigDataManager.h"


class CSystemDataModifyCheck
{
public:
	CSystemDataModifyCheck();
	~CSystemDataModifyCheck();

	void GetSystemData();
	void SetSystemDataModifyCheck();

protected:
	UINT SetSystemDataModifyCheck(PRIMX_SYSTEM_DATA& LastSystemData, PRIMX_SYSTEM_DATA& CurrentSystemData);

	PRIMX_SYSTEM_DATA			m_SystemLastData;
	CConfigDataManager			m_ConfigDataManager;

};

