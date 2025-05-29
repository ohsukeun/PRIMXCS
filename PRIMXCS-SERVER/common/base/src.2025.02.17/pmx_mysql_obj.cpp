/// @file pmx_mysql_obj.cpp  
/// @author Park, sung soo
/// @date 2023-06-12
/// @brief mysql database object methods

#include <time.h>

#include <pmx_mysql_obj.h>

#ifndef __PMX_MYSQL_OBJ_CPP__
#define __PMX_MYSQL_OBJ_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITIONS
//-----------------------------------------------------------------------------
#define __CATCH__ \
	catch (std::exception& eStd) \
	{ \
		snprintf(m_sErr, DB_ERR_STR_LEN, "[GENERIC ERROR] %s() on line %d : %s", \
			__FUNCTION__, __LINE__, eStd.what()); \
		m_lErr = DB_ERR_GENERIC; \
		m_bNativeErr = TRUE; \
	}

static bool CompareFieldIndex(CField* pPrev, CField* pNext)
{
	return (pPrev->GetID() < pNext->GetID());
}

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : CMyRSet creator & destroyer
    parameters  :
        - ()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
CMyRSet::CMyRSet()
{
	m_pRSet = NULL;
	m_Row   = NULL;
}

/*
CMyRSet::CMyRSet(IN MYSQL_RES* prset)
{
	m_pRSet = prset;
	m_Row   = NULL;
}
*/

CMyRSet::~CMyRSet()
{
	Close();
}

/* ----------------------------------------------------------------------------
    description : move first row
    parameters  :
			()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMyRSet::MoveFirstRow()
{
	mysql_data_seek(m_pRSet, 0);
}

/* ----------------------------------------------------------------------------
    description : fetch row
    parameters  :
			()
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMyRSet::FetchRow()
{
	if(NULL == m_pRSet) return FALSE;
	m_Row = mysql_fetch_row(m_pRSet);
	if(NULL == m_Row) return FALSE;
	return TRUE;
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. check if null given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMyRSet::IsNull(IN INT nField)
{
	if(NULL == m_pRSet) return TRUE;
	if(NULL == m_Row)   return TRUE;
	if(0 > nField) return TRUE;
	if(NULL == m_Row[nField]) return TRUE;
	return FALSE;
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get string data given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (CHAR*) return string
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
const CHAR* CMyRSet::GetStrValue(IN INT nField)
{
	if(NULL == m_pRSet) return (CHAR*)"";
	if(NULL == m_Row) return (CHAR*)"";
	if(0 > nField) return (CHAR*)"";
	return ((NULL == (CHAR*)m_Row[nField]) ? (CHAR*)"" : ((CHAR*)m_Row[nField]));
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned int given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (UINT) return unsigned long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT CMyRSet::GetUIntValue(IN INT nField)
{
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get int given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (INT) return long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CMyRSet::GetIntValue(IN INT nField)
{
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (INT)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned long given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (ULONG) return unsigned long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
ULONG CMyRSet::GetULongValue(IN INT nField)
{
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (ULONG)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get long given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (ULONG) return long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
LONG CMyRSet::GetLongValue(IN INT nField)
{
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (LONG)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned long long given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (ULONG) return unsigned long long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT64 CMyRSet::GetUInt64Value(IN INT nField)
{
#if defined(__LP64__) || defined(_LP64)
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT64)strtoul(pbuf, NULL, 10);
#else
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT64)strtoull(pbuf, NULL, 10);
#endif
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get DOUBLE given field number
                  (index started from zero)
    parameters  :
		- (INT) field index
    return      : (ULONG) return DOUBLE value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
DOUBLE CMyRSet::GetDFValue(IN INT nField)
{
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (DOUBLE)atof(pbuf);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. check if null given field name
    parameters  :
		- (const CHAR*) field name
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMyRSet::IsNull(IN const CHAR* sField)
{
	if(NULL == sField)  return TRUE;
	if(NULL == m_pRSet) return TRUE;
	if(NULL == m_Row)   return TRUE;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return TRUE;
	if(NULL == m_Row[nField]) return TRUE;
	return FALSE;
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get string data given field name
    parameters  :
		- (const CHAR*) field name
    return      : (CHAR*) return string
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
const CHAR* CMyRSet::GetStrValue(IN const CHAR* sField)
{
	if(NULL == sField) return (CHAR*)"";
	if(NULL == m_pRSet) return (CHAR*)"";
	if(NULL == m_Row) return (CHAR*)"";

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return (CHAR*)"";
	return ((NULL == (CHAR*)m_Row[nField]) ? (CHAR*)"" : ((CHAR*)m_Row[nField]));
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned int given field name
    parameters  :
		- (const CHAR*) field name
    return      : (UINT) return unsigned long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT CMyRSet::GetUIntValue(IN const CHAR* sField)
{
	CHAR* pbuf = NULL;

	if(NULL == sField) return 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get int given field name
    parameters  :
		- (const CHAR*) field name
    return      : (INT) return long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CMyRSet::GetIntValue(IN const CHAR* sField)
{
	CHAR* pbuf = NULL;

	if(NULL == sField) return 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (INT)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned long given field name
    parameters  :
		- (const CHAR*) field name
    return      : (ULONG) return unsigned long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
ULONG CMyRSet::GetULongValue(IN const CHAR* sField)
{
	CHAR* pbuf = NULL;

	if(NULL == sField) return 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (ULONG)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get long given field name
    parameters  :
		- (const CHAR*) field name
    return      : (LONG) return long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
LONG CMyRSet::GetLongValue(IN const CHAR* sField)
{
	CHAR* pbuf = NULL;

	if(NULL == sField) return 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (LONG)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned long long given field name
    parameters  :
		- (const CHAR*) field name
    return      : (ULONG) return unsigned long long value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT64 CMyRSet::GetUInt64Value(IN const CHAR* sField)
{
#if defined(__LP64__) || defined(_LP64)
	CHAR* pbuf = NULL;

	if(NULL == sField) return 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT64)strtoul(pbuf, NULL, 10);
#else
	CHAR* pbuf = NULL;

	if(NULL == sField) retrn 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;

	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT64)strtoull(pbuf, NULL, 10);
#endif
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get DOUBLE given field name
    parameters  :
		- (const CHAR*) field name
    return      : (DOUBLE) return DOUBLE value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
DOUBLE CMyRSet::GetDFValue(IN const CHAR* sField)
{
	CHAR* pbuf = NULL;

	if(NULL == sField) return 0;
	if(NULL == m_pRSet) return 0;
	if(NULL == m_Row) return 0;
	INT nField = GetFieldIndex(sField);
	if(0 > nField) return 0;
	pbuf = (CHAR*)m_Row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (DOUBLE)atof(pbuf);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. check if null given row number & field number
                  (index started from zero)
    parameters  :
        - (INT) row index
		- (INT) field index
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMyRSet::IsNull(IN INT nRow, IN INT nField)
{
	MYSQL_ROW row = NULL;

	if(NULL == m_pRSet) return TRUE;
	if((0 > nRow) || (0 > nField)) return TRUE;
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return TRUE;
	if(NULL == row[nField]) return TRUE;
	return FALSE;
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get string data given row number & field number
                  (index started from zero)
    parameters  :
        - (INT) row index
		- (INT) field index
    return      : (CHAR*) return string
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
CHAR* CMyRSet::GetStrValue(IN INT nRow, IN INT nField)
{
	MYSQL_ROW row = NULL;

	if(NULL == m_pRSet) return (CHAR*)"";
	if((0 > nRow) || (0 > nField)) return (CHAR*)"";
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return (CHAR*)"";
	return ((NULL == (CHAR*)row[nField]) ? (CHAR*)"" : ((CHAR*)row[nField]));
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get unsigned long data given row number & field number
                  (index started from zero)
    parameters  :
        - (INT) row index
		- (INT) field index
    return      : (ULONG) return value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
ULONG CMyRSet::GetULongValue(IN INT nRow, IN INT nField)
{
	MYSQL_ROW row = NULL;
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if((0 > nRow) || (0 > nField)) return 0;
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return 0;
	pbuf = (CHAR*)row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (ULONG)strtoul(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get long data given row number & field number
                  (index started from zero)
    parameters  :
        - (INT) row index
		- (INT) field index
    return      : (LONG) return value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
LONG CMyRSet::GetLongValue(IN INT nRow, IN INT nField)
{
	MYSQL_ROW row = NULL;
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if((0 > nRow) || (0 > nField)) return 0;
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return 0;
	pbuf = (CHAR*)row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (LONG)strtol(pbuf, NULL, 10);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get UINT64 data given row number & field number
                  (index started from zero)
    parameters  :
        - (INT) row index
		- (INT) field index
    return      : (UINT64) return value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT64 CMyRSet::GetUInt64Value(IN INT nRow, IN INT nField)
{
#if defined(__LP64__) || defined(_LP64)
	MYSQL_ROW row = NULL;
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if((0 > nRow) || (0 > nField)) return 0;
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return 0;
	pbuf = (CHAR*)row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT64)strtoul(pbuf, NULL, 10);
#else
	MYSQL_ROW row = NULL;
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if((0 > nRow) || (0 > nField)) return 0;
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return 0;
	pbuf = (CHAR*)row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (UINT64)strtoull(pbuf, NULL, 10);
#endif
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get DOUBLE float data given row number & field number
                  (index started from zero)
    parameters  :
        - (INT) row index
		- (INT) field index
    return      : (DOUBLE) return value
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
DOUBLE CMyRSet::GetDFValue(IN INT nRow, IN INT nField)
{
	MYSQL_ROW row = NULL;
	CHAR* pbuf = NULL;

	if(NULL == m_pRSet) return 0;
	if((0 > nRow) || (0 > nField)) return 0;
	mysql_data_seek(m_pRSet, nRow);
	if(NULL == (row = mysql_fetch_row(m_pRSet))) return 0;
	pbuf = (CHAR*)row[nField];
	if(NULL == pbuf) return 0;
	if(0 == strlen(pbuf)) return 0;
	return (DOUBLE)atof(pbuf);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get field count 
    parameters  :
		- ()
    return      : (INT) field count
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CMyRSet::GetFieldCount()
{
	if(NULL == m_pRSet) return 0;
	return (INT)mysql_num_fields(m_pRSet);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. get row count 
    parameters  :
		- ()
    return      : (INT) row count
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CMyRSet::GetRowCount()
{
	if(NULL == m_pRSet) return 0;
	return (INT)mysql_num_rows(m_pRSet);
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. close record set
    parameters  :
		- ()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMyRSet::Close()
{
	if(NULL == m_pRSet) return;
	mysql_free_result(m_pRSet);
	m_pRSet = NULL;
	if(0 < m_mapField.GetCount())
	{
		STR_TO_PTR_MAP_POSITION pos = m_mapField.GetStartPosition();

		while(false == m_mapField.IsEnd(pos))
		{
			WCString sKey;
			PVOID pOut = NULL;

			m_mapField.GetNextAssoc(pos, sKey, pOut);
			if(NULL != pOut)
			{
				CField* pDel = (CField*)pOut;
				delete pDel;
			}
		}
		m_mapField.RemoveAll();
	}
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. set recordset 
    parameters  :
		- (MYSQL_RES*) recordset pointer
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
/*
VOID CMyRSet::SetMYSQL_RES(IN MYSQL_RES* prset)
{
	if(NULL == prset) return; 
	m_pRSet = prset;
	SetFieldsToMap();
}
*/

/* ----------------------------------------------------------------------------
    description : CMyRSet. set recordset operator
    parameters  :
		- (MYSQL_RES*) recordset pointer
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
/*
VOID CMyRSet::operator =(IN MYSQL_RES* prset)
{
	if(NULL == prset) return; 
	m_pRSet = prset;
	SetFieldsToMap();
}
*/

/* ----------------------------------------------------------------------------
    description : CMyRSet. set recordset operator
    parameters  :
		- (CMyRSet*) recordset pointer
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
/*
VOID CMyRSet::operator =(IN CMyRSet* prset)
{
	if(NULL == prset) return; 
	m_pRSet = prset->m_pRSet;
	SetFieldsToMap();
}
*/

/* ----------------------------------------------------------------------------
    description : CMyRSet. set fields to the map
    parameters  :
        - ()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMyRSet::SetFieldsToMap()
{
	if(0 < m_mapField.GetCount()) m_mapField.RemoveAll();
	INT nCnt = (INT)mysql_num_fields(m_pRSet);
	for(INT nID = 0; nID < nCnt; nID++)
	{
		MYSQL_FIELD* pField = mysql_fetch_field_direct(m_pRSet, nID);
		if(NULL != pField)
		{
			if(NULL != pField->name)
			{
				WCString sKey(pField->name);
				CField*  pFieldInfo = new CField();
				INT      nDataType = pField->type;

				pFieldInfo->SetData(nID, sKey, nDataType, (UINT)pField->length);
				m_mapField.SetAt(sKey, (PVOID)pFieldInfo);
			}
		}
	}
}

/* ----------------------------------------------------------------------------
    description : CMyRSet. find index given field name
    parameters  :
        - (const CHAR*) field name
    return      : (INT) return field index
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CMyRSet::GetFieldIndex(IN const CHAR* sField)
{
	INT nRet = -1;
	if(NULL == sField) return nRet;
	if(0 < m_mapField.GetCount())
	{
		WCString sKey(sField);
		PVOID pOut = NULL;

		if(TRUE == m_mapField.Lookup(sKey, pOut))
		{
			if(NULL != pOut)
			{
				CField* pFieldInfo = (CField*)pOut;
				nRet = pFieldInfo->GetID();
			}
		}
	}
	return nRet;
}

/* ----------------------------------------------------------------------------
	description : CMyRSet. extract index based sorted field array 
	parameters  :
		- (WCArray<CField*>&) out array
	return      : (INT) return field count
	history     :
		- 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CMyRSet::GetFieldsSortedArray(IN WCArray<CField*>& arrField)
{
	INT nRet = 0;
	if(0 < m_mapField.GetCount())
	{
		// extract fields
		STR_TO_PTR_MAP_POSITION pos = m_mapField.GetStartPosition();

		while(false == m_mapField.IsEnd(pos))
		{
			WCString sKey;
			PVOID pOut = NULL;

			m_mapField.GetNextAssoc(pos, sKey, pOut);
			if(NULL != pOut)
			{
				CField* pFieldInfo = (CField*)pOut;
				arrField.Add(pFieldInfo);
			}
		}
		// sort array
		sort(arrField.m_Array.begin(), arrField.m_Array.end(), CompareFieldIndex);
	}
	nRet = (INT)arrField.GetCount();
	return nRet;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. creator & destroyer
    parameters  :
        - ()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
CMySQLDBObj::CMySQLDBObj()
{
	memset(m_sHost,   0x00, DB_HOST_LEN);
	memset(m_sDBName, 0x00, DB_NAME_LEN);
	memset(m_sUser,   0x00, DB_USER_LEN);
	memset(m_sPwd,    0x00, DB_PWD_LEN);
	m_bUseUTF8    = FALSE;
	m_bUseSSL     = FALSE;
	m_SSLKey.Empty();
	m_SSLCert.Empty();
	m_SSLCA.Empty();
	m_SSLCAPath.Empty();
	m_SSLCipher.Empty();
	m_nPort       = 0;
	m_sCharSet.Empty();
	m_pConn       = NULL;
	m_bConn       = FALSE;
	m_bAutoCommit = FALSE;
	m_bNativeErr  = FALSE;
	m_lErr        = 0;
	memset(m_sErr, 0x00, 512);
	m_bIsBusy     = FALSE;
	m_nID         = -1;
	time(&m_tLastQry);
	m_nAffectCnt  = 0;
	m_bTimeOut    = FALSE;
	memset(&m_tQryStartTm, 0x00, sizeof(struct timeb));
	memset(&m_tQryEndTm,   0x00, sizeof(struct timeb));
}

CMySQLDBObj::~CMySQLDBObj()
{
	if(NULL != m_pConn) Disconnect();
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj init.
    parameters  :
		- (const CHAR*) host name
		- (const CHAR*) database name
		- (const CHAR*) character set
		- (const CHAR*) NCharacter set
		- (const CHAR*) user
		- (const CHAR*) password 
		- (USHORT)      database port number
    return      : (SWC_BOOL) result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMySQLDBObj::Init(IN const CHAR* shost, 
                           IN const CHAR* sdbname,
						   IN const CHAR* sCharSet,
						   IN const CHAR* sNCharSet,
                           IN const CHAR* suser,
                           IN const CHAR* spwd,
                           IN USHORT      nport)
{
	try
	{
		if(NULL == shost)           { SetErr(TRUE, DB_ERR_EMPTY_HOST);   return FALSE; }
		if(NULL == sdbname)         { SetErr(TRUE, DB_ERR_EMPTY_DBNAME); return FALSE; }
		if(NULL == suser)           { SetErr(TRUE, DB_ERR_EMPTY_USER);   return FALSE; }
		if(NULL == spwd)            { SetErr(TRUE, DB_ERR_EMPTY_PWD);    return FALSE; }
		if(0    == nport)           { SetErr(TRUE, DB_ERR_PORT_ZERO);    return FALSE; }
		if(0    == strlen(shost))   { SetErr(TRUE, DB_ERR_EMPTY_HOST);   return FALSE; }
		if(0    == strlen(sdbname)) { SetErr(TRUE, DB_ERR_EMPTY_DBNAME); return FALSE; }
		if(0    == strlen(suser))   { SetErr(TRUE, DB_ERR_EMPTY_USER);   return FALSE; }
		if(0    == strlen(spwd))    { SetErr(TRUE, DB_ERR_EMPTY_PWD);    return FALSE; }

		snprintf(m_sHost,   DB_HOST_LEN, "%s", shost);
		snprintf(m_sDBName, DB_NAME_LEN, "%s", sdbname);
		snprintf(m_sUser,   DB_USER_LEN, "%s", suser);
		snprintf(m_sPwd,    DB_PWD_LEN,  "%s", spwd);
		m_nPort = nport;

		if(NULL != sCharSet)
		{
			WCString sChar = sCharSet;
			sChar = sChar.SpanExcluding('_');
			sChar = sChar.SpanExcluding('-');
			if     (0 <= sChar.FindNoCase(DEF_PMX_MYSQL_CHARSET_EUCKR))  m_sCharSet = DEF_PMX_MYSQL_CHARSET_EUCKR;
			else if(0 <= sChar.FindNoCase(DEF_PMX_MYSQL_CHARSET_ASCII))  m_sCharSet = DEF_PMX_MYSQL_CHARSET_ASCII;
			else if(0 <= sChar.FindNoCase(DEF_PMX_MYSQL_CHARSET_UTF8))   m_sCharSet = DEF_PMX_MYSQL_CHARSET_UTF8;
			else if(0 <= sChar.FindNoCase(DEF_PMX_MYSQL_CHARSET_LATIN1)) m_sCharSet = DEF_PMX_MYSQL_CHARSET_LATIN1;
		}
	}
	__CATCH__
	return TRUE;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. connect to the database
    parameters  :
        - (IN SWC_BOOL)    use ssl or not
		- (IN const CHAR*) ssl key
		- (IN const CHAR*) ssl cert file
		- (IN const CHAR*) ssl ca file
		- (IN const CHAR*) ssl ca file path
		- (IN const CHAR*) ssl cipher
    return      : (VOID)
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::InitSSL(IN SWC_BOOL    buse,
						  IN const CHAR* sslkey,
						  IN const CHAR* sslcert,
						  IN const CHAR* sslca,
						  IN const CHAR* sslcapath,
						  IN const CHAR* sslcipher)
{
	try
	{
		if(FALSE == buse) 
		{
			m_bUseSSL = FALSE;
			return;
		}
		if(NULL != sslkey)
		{
			if(0 < strlen(sslkey)) m_SSLKey = sslkey;
		}
		if(NULL != sslcert)
		{
			if(0 < strlen(sslcert)) m_SSLCert = sslcert;
		}
		if(NULL != sslca)
		{
			if(0 < strlen(sslca)) m_SSLCA = sslca;
		}
		if(NULL != sslcapath)
		{
			if(0 < strlen(sslcapath)) m_SSLCAPath = sslcapath;
		}
		if(NULL != sslcipher)
		{
			if(0 < strlen(sslcipher)) m_SSLCipher = sslcipher;
		}
		m_SSLKey.Trim();
		m_SSLCert.Trim();
		m_SSLCA.Trim();
		m_SSLCAPath.Trim();
		m_SSLCipher.Trim();
		if( (0 == m_SSLKey.GetLength()) && (0 == m_SSLCert.GetLength()) &&
			(0 == m_SSLCA.GetLength())  && (0 == m_SSLCAPath.GetLength()) &&
			(0 == m_SSLCipher.GetLength()))
		{
			m_bUseSSL = FALSE;
		}
		else
		{
			m_bUseSSL = TRUE;
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. connect to the database
    parameters  :
        - ()
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMySQLDBObj::Connect()
{
	SWC_BOOL bRet = FALSE;
	try
	{
		CHAR sbuf[300] = {0};
		
		if(NULL == m_pConn) 
		{ 
			if( (0 == strlen(m_sHost)) || (0 == strlen(m_sDBName)) ||
				(0 == strlen(m_sUser)) || (0 == strlen(m_sPwd)))
			{
				m_bConn = FALSE;
				SetErr(TRUE, DB_ERR_NULL_DB_PTR); 
				return FALSE;
			}
			else
			{
				m_pConn = (MYSQL*)malloc(sizeof(MYSQL));
				mysql_init(m_pConn);
				if( (0 == m_SSLKey.GetLength()) && (0 == m_SSLCert.GetLength()) &&
					(0 == m_SSLCA.GetLength())  && (0 == m_SSLCAPath.GetLength()) &&
					(0 == m_SSLCipher.GetLength()))
				{
					m_bUseSSL = FALSE;
				}
				else
				{
					mysql_ssl_set(m_pConn,
								(0 < m_SSLKey.GetLength())    ? LPCSTR(m_SSLKey)    : NULL,
								(0 < m_SSLCert.GetLength())   ? LPCSTR(m_SSLCert)   : NULL,
								(0 < m_SSLCA.GetLength())     ? LPCSTR(m_SSLCA)     : NULL,
								(0 < m_SSLCAPath.GetLength()) ? LPCSTR(m_SSLCAPath) : NULL,
								(0 < m_SSLCipher.GetLength()) ? LPCSTR(m_SSLCipher) : NULL);
					my_bool bVerifyCA = false;
					mysql_optionsv(m_pConn, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, (PVOID)&bVerifyCA);
					m_bUseSSL = TRUE;
				}
			}

			if(NULL == mysql_real_connect(m_pConn, m_sHost, m_sUser, m_sPwd, m_sDBName,
										m_nPort, NULL, 0 ))
			{
				SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
				free(m_pConn);
				m_pConn = NULL;
				m_bConn = FALSE;
				return FALSE;
			}
		}
		else
		{
			if(TRUE == m_bConn)
			{
				if(0 != mysql_ping(m_pConn))
				{
					SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
					m_bConn = FALSE;
					return FALSE;
				}
			}
		}
		snprintf(sbuf, 300, "USE %s; ", m_sDBName);
		if(0 != mysql_query(m_pConn, sbuf))
		{
			SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
			free(m_pConn);
			m_pConn = NULL;
			m_bConn = FALSE;
			return FALSE;
		}
		if(0 < m_sCharSet.GetLength())
		{
			snprintf(sbuf, 100, "set names %s; ", LPCSTR(m_sCharSet));
			if(0 != mysql_query(m_pConn, sbuf))
			{
				SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
				free(m_pConn);
				m_pConn = NULL;
				m_bConn = FALSE;
				return FALSE;
			}
		}
		if(TRUE == m_bUseUTF8)
		{
			snprintf(sbuf, 100, "set names utf8; ");
			if(0 != mysql_query(m_pConn, sbuf))
			{
				SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
				free(m_pConn);
				m_pConn = NULL;
				m_bConn = FALSE;
				return FALSE;
			}
		}
		if(0 != mysql_commit(m_pConn))
		{
			SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
			free(m_pConn);
			m_pConn = NULL;
			m_bConn = FALSE;
			return FALSE;
		}
		SetErr(FALSE, 0);
		m_bConn = TRUE;
		m_bIsBusy      = FALSE;
		m_tLastQry     = 0;
		m_bNativeErr   = FALSE;
		m_lErr         = 0;
		m_lErrDatabase = 0;
		m_bErrCritical = FALSE;
		m_bErrDeadLock = FALSE;
		memset(m_sErr, 0x00, DB_ERR_STR_LEN);

		_LOG_DBG("DATABASE CONNECTION SUCCESSED");

		// for debuging ...
		//PrintMYSQL(m_pConn);

		bRet = TRUE;
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. disconnect from database
    parameters  :
        - ()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::Disconnect()
{
	if(NULL == m_pConn) return;
	try
	{
		_LOG_DBG("DATABASE DISCONNECTED");

		mysql_close(m_pConn);
		m_pConn        = NULL;
		m_bConn        = FALSE;
		m_bIsBusy      = FALSE;
		m_tLastQry     = 0;
		m_bNativeErr   = FALSE;
		m_lErr         = 0;
		m_bErrCritical = FALSE;
		m_bErrDeadLock = FALSE;
		memset(m_sErr, 0x00, DB_ERR_STR_LEN);
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. commit transaction
    parameters  :
        - ()
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMySQLDBObj::Commit()
{
	SWC_BOOL bRet = FALSE;
	try
	{
		if(NULL == m_pConn) return FALSE;
		if(0 != mysql_commit(m_pConn))
		{
			SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
			return FALSE;
		}
		bRet = TRUE;
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. rollback transaction
    parameters  :
        - ()
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMySQLDBObj::Rollback()
{
	if(NULL == m_pConn) return FALSE;
	try
	{
		if(TRUE == m_bAutoCommit)
		{
			SetErr(TRUE, DB_ERR_AUTO_COMMIT);
			return FALSE;
		}
		if(0 != mysql_rollback(m_pConn))
		{
			SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
			return FALSE;
		}
	}
	__CATCH__
	return TRUE;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. rollback transaction
    parameters  :
        - (SWC_BOOL) set auto commit or not
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::SetAutoCommit(IN SWC_BOOL bset)
{
	try
	{
		if(NULL == m_pConn) return;
		mysql_autocommit(m_pConn, bset);
		m_bAutoCommit = bset;
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. execute a query
    parameters  :
		- (SWC_BOOL) select(TRUE) or insert/update/delete(FALSE) query
        - (WCString) query string 
		- (SWC_BOOL) use array fetch or not (oracle only)
    return      : (CMyRSet*)
    history     :
		- 2018. 01. 23 modified by Park, Sung soo
---------------------------------------------------------------------------- */
CMyRSet* CMySQLDBObj::ExecuteQuery(IN SWC_BOOL brset, IN WCString sqry, IN SWC_BOOL bUseArrayFetch)
{
	return ExecuteQuery(brset, LPCSTR(sqry));
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. execute a query
    parameters  :
		- (SWC_BOOL)    select(TRUE) or insert/update/delete(FALSE) query
        - (const CHAR*) query string 
		- (SWC_BOOL) use array fetch or not (oracle only)
    return      : (CMyRSet*)
    history     :
		- 2018. 01. 23 modified by Park, Sung soo
---------------------------------------------------------------------------- */
CMyRSet* CMySQLDBObj::ExecuteQuery(IN SWC_BOOL brset, IN const CHAR* sqry, IN SWC_BOOL bUseArrayFetch)
{
	CMyRSet* pret = NULL;
	try
	{
		MYSQL_RES* pres = NULL;
		time_t tcur = 0;
		
		if(NULL == sqry)      { SetErr(TRUE, DB_ERR_EMPTY_QRY); return NULL; }
		if(0 == strlen(sqry)) { SetErr(TRUE, DB_ERR_EMPTY_QRY); return NULL; }
		time(&tcur);
		m_tLastQry = tcur;
		WCString sfix(sqry);
		//FixString(sfix);

		if(FALSE == m_bConn) 
		{ 
			if(FALSE == TryReconnect()) return NULL;
		}
		if(TRUE == m_bErrCritical) 
		{ 
			m_bConn = FALSE; 
			if(FALSE == TryReconnect()) return NULL;
		}
		if(0 != RunQuery(LPCSTR(sfix)))
		{
			if(TRUE == m_bTimeOut)
			{
				m_bTimeOut = FALSE;
				TryReconnect();
				SetErr(TRUE, DB_ERR_TIMEOUT);
				return NULL;
			}
			else
			{
				SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
				if(TRUE == m_bErrCritical)
				{
					if(TRUE == TryReconnect())
					{
						if(0 != RunQuery(LPCSTR(sfix))) 
						{ 
							SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn)); 
							return NULL;
						}
					}
				}
				else 
				{
					SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn)); 
					return NULL;
				}
			}
		}
		if(TRUE == brset)
		{
			if(NULL == (pres = mysql_store_result(m_pConn)))
			{
				SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
				return NULL;
			}
			pret = new CMyRSet;
			pret->m_pRSet = pres;
			pret->SetFieldsToMap();
		}
		else
		{
			m_nAffectCnt = mysql_affected_rows(m_pConn);
		}
		SetErr(FALSE, DB_ERR_SUCCESS);
	}
	__CATCH__
	return pret;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. run a query and calculate query running time
    parameters  :
        - (const CHAR*) query string 
    return      : (INT) query result
    history     :
		- 2018. 01. 23 created by Park, Sung soo
		- 2023. 06. 13 ftime() deprecated. changed clock_gettime()
---------------------------------------------------------------------------- */
INT CMySQLDBObj::RunQuery(IN const CHAR* sQry)
{
	INT nRet = 0;
	try
	{
		clock_gettime(CLOCK_MONOTONIC, &m_tQryStartTm);
		nRet = mysql_query(m_pConn, sQry);
		clock_gettime(CLOCK_MONOTONIC, &m_tQryEndTm);
	}
	__CATCH__
	return nRet;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. get last error number
    parameters  :
		- ()
    return      : (ULONG) return error nunmber
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
ULONG CMySQLDBObj::GetLastErrorNumber()
{
	return m_lErr;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. get error message
    parameters  :
		- ()
    return      : (const CHAR*)
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
const CHAR* CMySQLDBObj::GetLastErrorString()
{
	if(TRUE == m_bNativeErr)
	{
		snprintf(m_sErr, DB_ERR_STR_LEN, "success");

		if     (DB_ERR_EMPTY_DBNAME       == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty database name");
		else if(DB_ERR_EMPTY_USER         == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty database user");
		else if(DB_ERR_EMPTY_PWD          == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty database password");
		else if(DB_ERR_EMPTY_HOST         == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty database host");
		else if(DB_ERR_PORT_ZERO          == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "database port number is zero");
		else if(DB_ERR_EMPTY_CONNSTR      == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty connection string");
		else if(DB_ERR_NULL_DB_PTR        == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "null database object pointer");
		else if(DB_ERR_AUTO_COMMIT        == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "auto-commit mode");
		else if(DB_ERR_EMPTY_QRY          == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty query");
		else if(DB_ERR_TIMEOUT            == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "query timeout");
		else if(DB_ERR_NULL_RSET_PTR      == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "null record set");
		else if(DB_ERR_GENERIC            == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "generic error");
		else if(DB_ERR_DATABASE           == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "database error");
		else if(DB_ERR_EMPTY_SID          == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty sid");
		else if(DB_ERR_EMPTY_SVCNAME      == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "empty service name");
		else if(DB_ERR_NULL_ENV_PTR       == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "null occi environment pointer");
		else if(DB_ERR_NULL_CONN_POOL_PTR == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "null connection pool pointer");
		else if(DB_ERR_NULL_CONN_PTR      == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "null connection pointer");
		else if(DB_ERR_OUT_OF_RANGE       == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "out of range of number type");
		else if(DB_ERR_INIT               == m_lErr) snprintf(m_sErr, DB_ERR_STR_LEN, "not initialized");
	}
	else
	{
		if(1045 == m_lErr)
		{
			snprintf(m_sErr, DB_ERR_STR_LEN, "Invalid user name and/or password.");
		}
		else
		{
			return LPCSTR(m_sLastErr);
		}
	}
	return m_sErr;
}

/* ----------------------------------------------------------------------------
    description : CMySQLDBObj. set error number
    parameters  :
		- (IN SWC_BOOL)    native error or not
		- (IN ULONG)       error number
		- (IN const CHAR*) last error string
    return      : 
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::SetErr(IN SWC_BOOL bnative, IN ULONG lerr, IN const CHAR* sLastErr)
{
	try
	{
		m_bNativeErr = bnative;

		if(DB_ERR_SUCCESS == lerr)
		{
			m_bNativeErr   = FALSE;
			m_lErr         = 0;
			m_lErrDatabase = 0;
			m_bErrCritical = FALSE;
			m_bErrDeadLock = FALSE;
			memset(m_sErr, 0x00, DB_ERR_STR_LEN);
			m_sLastErr.Empty();
			return;
		}

		if(TRUE == bnative)
		{
			m_lErr = lerr;
		}
		else
		{
			m_lErr = DB_ERR_DATABASE;
			m_lErrDatabase = lerr;
		}
		if( (1042 == lerr) || (1043 == lerr) || (1053 == lerr) || (1077 == lerr) ||
			(1079 == lerr) || (1081 == lerr) || (1081 == lerr) || (1129 == lerr) ||
			(1030 == lerr) || (1158 == lerr) || (1159 == lerr) || (1160 == lerr) ||
			(1161 == lerr) || (2002 == lerr) || (2003 == lerr) || (2004 == lerr) ||
			(2005 == lerr) || (2006 == lerr) || (2014 == lerr) || (2013 == lerr) ||
			(2055 == lerr))
		{
			m_bErrCritical = TRUE;
			m_bErrDeadLock = FALSE;
		}
		else if((1213 == lerr) || (1479 == lerr))
		{
			m_bErrCritical = FALSE;
			m_bErrDeadLock = TRUE;
		}
		else
		{
			m_bErrCritical = FALSE;
			m_bErrDeadLock = FALSE;
		}
		if(NULL != sLastErr) m_sLastErr = sLastErr;
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
    description : fix character for mysql
    parameters  :
        - (WCString&) string
    return      :
    history     :
        - 2014. 04. 29 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::FixString(OUT WCString& sqry)
{
    if(0 == sqry.GetLength()) return;
    sqry.Replace(92, 92, 2); // '\' -> '\\' for mysql
	//sqry.Replace("\\'", "\'");
}

/* ----------------------------------------------------------------------------
    description : set character set 
    parameters  :
        - (const CHAR*) character set string
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::SetUseCharSet(IN const CHAR* sset)
{
	if(NULL == sset) return;
	CHAR sbuf[100] = {0};

	try
	{
		snprintf(sbuf, 100, "set names %s; ", sset);
		if(0 != mysql_query(m_pConn, sbuf))
		{
			SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
		}
		if(0 != mysql_commit(m_pConn))
		{
			SetErr(FALSE, mysql_errno(m_pConn), mysql_error(m_pConn));
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
    description : try reconnection
    parameters  :
        - ()
    return      : (SWC_BOOL) return result
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CMySQLDBObj::TryReconnect()
{
	UINT nCnt = 0;

	while(nCnt < DB_MAX_RETRY_CONN)
	{
		if(TRUE == Connect()) 
		{ 
			_LOG_MSG("reconnect successed"); 
			return TRUE; 
		}
		nCnt++;

		struct timespec req;
		req.tv_sec = 2;
		req.tv_nsec = 0;
		nanosleep(&req, &req);

		_LOG_ERR("reconnecting failed");
	}
	return FALSE;
}

/* ----------------------------------------------------------------------------
    description : get query time
    parameters  :
        - (UINT&) out seconds
		- (UINT&) out mili-seconds
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::GetQueryRunTime(OUT UINT& nSec, OUT UINT& nMili)
{
	nSec  = (UINT)abs(m_tQryEndTm.tv_sec - m_tQryStartTm.tv_sec);
	nMili = (UINT)abs((m_tQryEndTm.tv_nsec - m_tQryStartTm.tv_nsec) / 1000000);
}

VOID CMySQLDBObj::SetTimeOuted()
{
	m_bTimeOut = TRUE;
}

/* ----------------------------------------------------------------------------
    description : get query time
    parameters  :
        - (UINT&) out seconds
		- (UINT&) out mili-seconds
    return      :
    history     :
        - 2018. 01. 23 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CMySQLDBObj::PrintMYSQL(IN MYSQL* pMysql)
{
	if(NULL == pMysql) return;
	try
	{
		_LOG_DBG("-----------------------------------------------------------------");
		_LOG_DBG("MYSQL structure info.");
		_LOG_DBG("host                = %s",   pMysql->host);
		_LOG_DBG("user                = %s",   pMysql->user);
		_LOG_DBG("passwd              = %s",   pMysql->unix_socket);
		_LOG_DBG("server_version      = %s",   pMysql->server_version);
		_LOG_DBG("host_info           = %s",   pMysql->host_info);
		_LOG_DBG("port                = %u",   pMysql->port);
		_LOG_DBG("client_flag         = %lu",  pMysql->client_flag);
		_LOG_DBG("server_capabiliyies = %lu",  pMysql->server_capabilities);
		_LOG_DBG("protocol_version    = %u",   pMysql->protocol_version);
		_LOG_DBG("field_count         = %u",   pMysql->field_count);
		_LOG_DBG("thread_id           = %lu",  pMysql->thread_id);
		_LOG_DBG("affected_rows       = %llu", pMysql->affected_rows);
		_LOG_DBG("insert_id           = %llu", pMysql->insert_id);
		_LOG_DBG("extra_info          = %llu", pMysql->extra_info);
		_LOG_DBG("packet_length       = %lu",  pMysql->packet_length);
		_LOG_DBG("mysql status        = %u",   pMysql->status);
		_LOG_DBG("free_me             = %u",   pMysql->free_me);
	}
	__CATCH__
}

//-----------------------------------------------------------------------------
#endif // __PMX_MYSQL_OBJ_CPP__
