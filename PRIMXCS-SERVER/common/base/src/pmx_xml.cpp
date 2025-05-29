/// @file xma_xml.cpp
/// @author Park, sung soo
/// @date 2023-06-12
/// @brief xml I/O hamdling class

#include <pmx_xml.h>

#ifndef __PMX_XML_CPP__
#define __PMX_XML_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINTION
//-----------------------------------------------------------------------------
std::string _PMX_ERR_XML_STR[] = 
{
	"no error",
	"unknown error", 
	"invalid xml file path",
	"xml file open failed",
	"AddElem() failed",
	"IntoElem() failed",
	"OutOfElem() failed",
	"FindElem() failed",
	"SetData() failed",
	"Save() failed",
	"SetDoc() failed",
	"empty xml file",
	"empty or null buffer",
	"too many items",
	"null input data",
	"invalid input data"
};

#define __CATCH__ \
	catch(std::exception& e) \
	{ \
		_LOG_ERR("%s", e.what()); \
	}

#define __CATCH_RETURN_(X) \
	catch(std::exception& e) \
	{ \
		_LOG_ERR("%s", e.what()); \
		return X; \
	}

#define __CATCH_XML__ \
	catch(_PMX_ERR_XML_RW eXml) \
	{ \
		MakeLastError(m_Markup, eXml); \
	}

#define __CATCH_XML_RETURN_(X) \
	catch(_PMX_ERR_XML_RW eXml) \
	{ \
		MakeLastError(m_Markup, eXml); \
		return X; \
	}

//-----------------------------------------------------------------------------
// METHODS DEFINTION
//-----------------------------------------------------------------------------
CPmxXml::CPmxXml()
{
	m_Markup.SetDocFlags(8); // MarkupDocFlags.MDF_IGNORECASE
}

CPmxXml::CPmxXml(WCString sXml)
{
	m_Markup.SetDocFlags(8); // MarkupDocFlags.MDF_IGNORECASE
	SetXml(sXml);
}

CPmxXml::~CPmxXml()
{
}

SWC_BOOL CPmxXml::SetXml(WCString sXml)
{
	SWC_BOOL bRet = TRUE;
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sXml.GetLength())                       throw PMX_ERR_XML_RW_EMPTY_NULL_BUFFER;
		if(FALSE == m_Markup.SetDoc(sXml.m_Str.data())) throw PMX_ERR_XML_RW_SETDOC_FAILED;
	}
	__CATCH_XML_RETURN_(FALSE)
	__CATCH_RETURN_(FALSE)
	return bRet;
}

WCString CPmxXml::GetElmValueStrByPath (IN WCString sPath)
{
	WCString sRet;
	sRet.Empty();
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                         throw PMX_ERR_XML_RW_INVALID_PATH;
		if(FALSE == m_Markup.FindElem(sPath.m_Str.data())) throw PMX_ERR_XML_RW_FINDELM_FAILED;
		MCD_STR sBuf = m_Markup.GetData();
		sRet = sBuf.data();
	}
	__CATCH_XML__
	__CATCH__
	return sRet;
}

UINT CPmxXml::GetElmValueUIntByPath (IN WCString sPath)
{
	UINT nRet = 0;
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                         throw PMX_ERR_XML_RW_INVALID_PATH;
		if(FALSE == m_Markup.FindElem(sPath.m_Str.data())) throw PMX_ERR_XML_RW_FINDELM_FAILED;
		MCD_STR sBuf = m_Markup.GetData();
		nRet = (UINT)atoi(sBuf.data());
	}
	__CATCH_XML__
	__CATCH__
	return nRet;
}

INT CPmxXml::GetElmValueIntByPath (IN WCString sPath)
{
	UINT nRet = 0;
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                         throw PMX_ERR_XML_RW_INVALID_PATH;
		if(FALSE == m_Markup.FindElem(sPath.m_Str.data())) throw PMX_ERR_XML_RW_FINDELM_FAILED;
		MCD_STR sBuf = m_Markup.GetData();
		nRet = (INT)atoi(sBuf.data());
	}
	__CATCH_XML__
	__CATCH__
	return nRet;
}

SWC_BOOL CPmxXml::GetElmValueBoolByPath (IN WCString sPath)
{
	SWC_BOOL bRet = 0;
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                         throw PMX_ERR_XML_RW_INVALID_PATH;
		if(FALSE == m_Markup.FindElem(sPath.m_Str.data())) throw PMX_ERR_XML_RW_FINDELM_FAILED;
		MCD_STR sBuf = m_Markup.GetData();
		bRet = (SWC_BOOL)(1 == atoi(sBuf.data()) ? TRUE : FALSE);
	}
	__CATCH_XML__
	__CATCH__
	return bRet;
}

WCString CPmxXml::GetAttrValueStrByPath(IN WCString sPath, IN WCString sAttr)
{
	WCString sRet;
	sRet.Empty();
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                         throw PMX_ERR_XML_RW_INVALID_PATH;
		if(FALSE == m_Markup.FindElem(sPath.m_Str.data())) throw PMX_ERR_XML_RW_FINDELM_FAILED;
		MCD_STR sBuf = m_Markup.GetAttrib(sAttr.m_Str.data());
		sRet = sBuf.data();
	}
	__CATCH_XML__
	__CATCH__
	return sRet;
}

SWC_BOOL CPmxXml::IsExistElement(IN WCString sPath)
{
	SWC_BOOL bRet = FALSE;
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                        throw PMX_ERR_XML_RW_INVALID_PATH;
		if(TRUE == m_Markup.FindElem(sPath.m_Str.data())) bRet = TRUE; 
		else                                              throw PMX_ERR_XML_RW_FINDELM_FAILED; 
	}
	__CATCH_XML__
	__CATCH__
	return bRet;
}

VOID CPmxXml::SetElmValueStrByPath(IN WCString sPath, IN WCString sValue)
{
	try
	{
		MakeLastError(m_Markup, PMX_ERR_XML_RW_SUCCESS);
		if(0 == sPath.GetLength())                         throw PMX_ERR_XML_RW_INVALID_PATH;
		if(FALSE == m_Markup.FindElem(sPath.m_Str.data())) throw PMX_ERR_XML_RW_FINDELM_FAILED;
		m_Markup.SetData(sValue.m_Str.data());
	}
	__CATCH_XML__
	__CATCH__
}

WCString CPmxXml::ToStr()
{
	WCString sRet = (CHAR*)"";
	try
	{
		sRet = m_Markup.GetDoc();
	}
	__CATCH__
	return sRet;
}

VOID CPmxXml::MakeLastError(CMarkup& xml, UINT nErrNo)
{
	try
	{
		switch(nErrNo)
		{
			case PMX_ERR_XML_RW_SUCCESS :
				m_nLastError = nErrNo;
				break;
			case PMX_ERR_XML_RW_INVALID_PATH :
				snprintf(m_sLastError, PMX_XML_MAX_ERR_STR_LEN, "%s", _PMX_ERR_XML_STR[nErrNo].data());
				m_nLastError = nErrNo;
				break;
			case PMX_ERR_XML_RW_XML_OPEN_FAILED :
			case PMX_ERR_XML_RW_ADDELEM_FAILED :
			case PMX_ERR_XML_RW_INTOELEM_FAILED :
			case PMX_ERR_XML_RW_OUTOFELEM_FAILED :
			case PMX_ERR_XML_RW_FINDELM_FAILED :
			case PMX_ERR_XML_RW_SETDATA_FAILED :
			case PMX_ERR_XML_RW_SAVE_FAILED :
			case PMX_ERR_XML_RW_SETDOC_FAILED :
			case PMX_ERR_XML_RW_EMPTY_XML :
			case PMX_ERR_XML_RW_EMPTY_NULL_BUFFER :
			case PMX_ERR_XML_RW_NULL_INPUT_DATA :
			case PMX_ERR_XML_RW_INVALID_INPUT_DATA :
				snprintf(m_sLastError, PMX_XML_MAX_ERR_STR_LEN, "%s. native_err_str:%s", 
						_PMX_ERR_XML_STR[nErrNo].data(),
						xml.GetError().data());
				m_nLastError = nErrNo;
				break;

			default :
				snprintf(m_sLastError, PMX_XML_MAX_ERR_STR_LEN, "%s", _PMX_ERR_XML_STR[PMX_ERR_XML_RW_UNKNOWN_ERROR].data());
				m_nLastError = PMX_ERR_XML_RW_UNKNOWN_ERROR;
				break;
		}
	}
	__CATCH__
}

//-----------------------------------------------------------------------------
#endif // __PMX_XML_CPP__
