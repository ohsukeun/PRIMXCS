/// @file pmx_xml.h
/// @author Park, sung soo
/// @date 2023-06-12
/// @brief xml I/O handling class

#include <Markup.h>
#include <pmx_base_def.h>

#ifndef __PXM_XML_H__
#define __PXM_XML_H__
//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#ifndef MNF_WITHCDATA
	#define MNF_WITHCDATA 0x01
#endif

// length define
#define PMX_XML_MAX_ERR_STR_LEN  500

/// xml I/O errors
enum _PMX_ERR_XML_RW
{
	PMX_ERR_XML_RW_SUCCESS         = 0,
	PMX_ERR_XML_RW_UNKNOWN_ERROR,
	PMX_ERR_XML_RW_INVALID_PATH,
	PMX_ERR_XML_RW_XML_OPEN_FAILED,
	PMX_ERR_XML_RW_ADDELEM_FAILED,
	PMX_ERR_XML_RW_INTOELEM_FAILED,
	PMX_ERR_XML_RW_OUTOFELEM_FAILED,
	PMX_ERR_XML_RW_FINDELM_FAILED,
	PMX_ERR_XML_RW_SETDATA_FAILED,
	PMX_ERR_XML_RW_SAVE_FAILED,
	PMX_ERR_XML_RW_SETDOC_FAILED,
	PMX_ERR_XML_RW_EMPTY_XML,
	PMX_ERR_XML_RW_EMPTY_NULL_BUFFER,
	PMX_ERR_XML_RW_TOO_MANY_ITEMS,
	PMX_ERR_XML_RW_NULL_INPUT_DATA,
	PMX_ERR_XML_RW_INVALID_INPUT_DATA, 
};

/// xml I/O handling class
class CPmxXml
{
public:
	SWC_BOOL SetXml(IN WCString sXml);

	WCString GetElmValueStrByPath (IN WCString sPath);
	UINT     GetElmValueUIntByPath(IN WCString sPath);
	INT      GetElmValueIntByPath (IN WCString sPath);
	SWC_BOOL GetElmValueBoolByPath(IN WCString sPath);
	WCString GetAttrValueStrByPath(IN WCString sPath, IN WCString sAttr);
	SWC_BOOL IsExistElement       (IN WCString sPath);
	WCString ToStr                ();
	VOID     MakeLastError        (CMarkup& xml, UINT nErrNo);

	VOID     SetElmValueStrByPath(IN WCString sPath, IN WCString sValue);

	UINT        GetLastErrNo () { return m_nLastError; }
	const CHAR* GetLastErrStr() { return m_sLastError; }

	CPmxXml();
	CPmxXml(IN WCString sXml);
	virtual ~CPmxXml();

public: 
	CMarkup m_Markup;


private:
	UINT    m_nLastError;
	CHAR    m_sLastError[PMX_XML_MAX_ERR_STR_LEN];
};

//-----------------------------------------------------------------------------
#endif // __PXM_XML_H__
