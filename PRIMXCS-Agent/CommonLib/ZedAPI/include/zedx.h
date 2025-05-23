

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for ZedX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ZedX_h__
#define __ZedX_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IZedContainerProgress_FWD_DEFINED__
#define __IZedContainerProgress_FWD_DEFINED__
typedef interface IZedContainerProgress IZedContainerProgress;

#endif 	/* __IZedContainerProgress_FWD_DEFINED__ */


#ifndef __DispZedContainer_FWD_DEFINED__
#define __DispZedContainer_FWD_DEFINED__
typedef interface DispZedContainer DispZedContainer;

#endif 	/* __DispZedContainer_FWD_DEFINED__ */


#ifndef __IZedContainer_FWD_DEFINED__
#define __IZedContainer_FWD_DEFINED__
typedef interface IZedContainer IZedContainer;

#endif 	/* __IZedContainer_FWD_DEFINED__ */


#ifndef __IZedContainer2_FWD_DEFINED__
#define __IZedContainer2_FWD_DEFINED__
typedef interface IZedContainer2 IZedContainer2;

#endif 	/* __IZedContainer2_FWD_DEFINED__ */


#ifndef __DispZedContainer_FWD_DEFINED__
#define __DispZedContainer_FWD_DEFINED__
typedef interface DispZedContainer DispZedContainer;

#endif 	/* __DispZedContainer_FWD_DEFINED__ */


#ifndef __IZedContainer_FWD_DEFINED__
#define __IZedContainer_FWD_DEFINED__
typedef interface IZedContainer IZedContainer;

#endif 	/* __IZedContainer_FWD_DEFINED__ */


#ifndef __IZedContainer2_FWD_DEFINED__
#define __IZedContainer2_FWD_DEFINED__
typedef interface IZedContainer2 IZedContainer2;

#endif 	/* __IZedContainer2_FWD_DEFINED__ */


#ifndef __ZedContainer_FWD_DEFINED__
#define __ZedContainer_FWD_DEFINED__

#ifdef __cplusplus
typedef class ZedContainer ZedContainer;
#else
typedef struct ZedContainer ZedContainer;
#endif /* __cplusplus */

#endif 	/* __ZedContainer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_ZedX_0000_0000 */
/* [local] */ 

/*------------------------------------------------------------------------------
ZED>API - Version 2021     --Mark:$ZEDAPIVERSION$
  
Product(s)  ZONECENTRAL, ZEDMAIL, ZED! FULL EDITION
Platforms   Windows 7, 8.1, 10 
File:       ZedX.h
            ActiveX Component with both custom and IDispatch interfaces.
  
See also <zedapi.h> for various flags values and more help and information.
  
Copyright Prim'X Technologies SA 2004, 2014.
------------------------------------------------------------------------------*/
  
/*------------------------------------------------------------------------------
IZedContainerProgress
------------------------------------------------------------------------------*/


extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0000_v0_0_s_ifspec;

#ifndef __IZedContainerProgress_INTERFACE_DEFINED__
#define __IZedContainerProgress_INTERFACE_DEFINED__

/* interface IZedContainerProgress */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IZedContainerProgress;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("000001F0-8804-4CA8-8868-36F59DEFD14D")
    IZedContainerProgress : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FileProgress( 
            /* [in] */ LONGLONG Size,
            /* [in] */ LONGLONG Done) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IZedContainerProgressVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZedContainerProgress * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZedContainerProgress * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZedContainerProgress * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IZedContainerProgress * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IZedContainerProgress * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IZedContainerProgress * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IZedContainerProgress * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FileProgress )( 
            IZedContainerProgress * This,
            /* [in] */ LONGLONG Size,
            /* [in] */ LONGLONG Done);
        
        END_INTERFACE
    } IZedContainerProgressVtbl;

    interface IZedContainerProgress
    {
        CONST_VTBL struct IZedContainerProgressVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZedContainerProgress_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IZedContainerProgress_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IZedContainerProgress_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IZedContainerProgress_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IZedContainerProgress_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IZedContainerProgress_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IZedContainerProgress_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IZedContainerProgress_FileProgress(This,Size,Done)	\
    ( (This)->lpVtbl -> FileProgress(This,Size,Done) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IZedContainerProgress_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_ZedX_0000_0001 */
/* [local] */ 

/*------------------------------------------------------------------------------
DispZedContainer
------------------------------------------------------------------------------*/


extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0001_v0_0_s_ifspec;

#ifndef __DispZedContainer_INTERFACE_DEFINED__
#define __DispZedContainer_INTERFACE_DEFINED__

/* interface DispZedContainer */
/* [object][helpstring][uuid][nonextensible][dual][hidden] */ 


EXTERN_C const IID IID_DispZedContainer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("00001035-8804-4CA8-8868-36F59DEFD14D")
    DispZedContainer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateContainer( 
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenContainer( 
            /* [in] */ BSTR Name,
            /* [in] */ ULONG Flags,
            /* [in] */ BSTR CredentialName,
            /* [in] */ BSTR CredentialSecret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseContainer( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveFile( 
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteFile( 
            /* [in] */ BSTR ExistingFilename) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateFolder( 
            /* [in] */ BSTR NewFolderName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddFile( 
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtractFile( 
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileAttributes( 
            /* [in] */ BSTR ExistingFilename,
            /* [retval][out] */ LONG *Attributes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileAttributesEx( 
            /* [in] */ BSTR ExistingFilename,
            /* [out] */ LONG *Attributes,
            /* [out] */ DATE *CreationTime,
            /* [out] */ DATE *LastWriteTime,
            /* [out] */ DATE *LastAccessTime,
            /* [out] */ LONGLONG *FileSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindFirst( 
            /* [in] */ BSTR SearchFilename,
            /* [out] */ LONG *FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindNext( 
            /* [in] */ LONG FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindClose( 
            /* [in] */ LONG FindContext) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessPassword( 
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessCertificate( 
            /* [in] */ BSTR CerFile,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveAccess( 
            /* [in] */ BSTR AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAccessInfo( 
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNbAccesses( 
            /* [retval][out] */ LONG *Count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAccess( 
            /* [in] */ LONG Index,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FormatMessage( 
            /* [in] */ HRESULT hrCode,
            /* [retval][out] */ BSTR *Message) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetProgressObject( 
            /* [in] */ IZedContainerProgress *Progress) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessPassword2( 
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessCertificate2( 
            /* [in] */ BSTR CerFile,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessIndirect( 
            /* [in] */ BSTR Path,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAccessInfo2( 
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate,
            /* [out] */ LONG *Role,
            /* [out] */ LONG *Attributes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeAccessRole( 
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Role) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeAccessAttributes( 
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Attributes) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct DispZedContainerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            DispZedContainer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            DispZedContainer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            DispZedContainer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            DispZedContainer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            DispZedContainer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            DispZedContainer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            DispZedContainer * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateContainer )( 
            DispZedContainer * This,
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenContainer )( 
            DispZedContainer * This,
            /* [in] */ BSTR Name,
            /* [in] */ ULONG Flags,
            /* [in] */ BSTR CredentialName,
            /* [in] */ BSTR CredentialSecret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseContainer )( 
            DispZedContainer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveFile )( 
            DispZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteFile )( 
            DispZedContainer * This,
            /* [in] */ BSTR ExistingFilename);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateFolder )( 
            DispZedContainer * This,
            /* [in] */ BSTR NewFolderName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddFile )( 
            DispZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtractFile )( 
            DispZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileAttributes )( 
            DispZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [retval][out] */ LONG *Attributes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileAttributesEx )( 
            DispZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [out] */ LONG *Attributes,
            /* [out] */ DATE *CreationTime,
            /* [out] */ DATE *LastWriteTime,
            /* [out] */ DATE *LastAccessTime,
            /* [out] */ LONGLONG *FileSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindFirst )( 
            DispZedContainer * This,
            /* [in] */ BSTR SearchFilename,
            /* [out] */ LONG *FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindNext )( 
            DispZedContainer * This,
            /* [in] */ LONG FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindClose )( 
            DispZedContainer * This,
            /* [in] */ LONG FindContext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessPassword )( 
            DispZedContainer * This,
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessCertificate )( 
            DispZedContainer * This,
            /* [in] */ BSTR CerFile,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAccess )( 
            DispZedContainer * This,
            /* [in] */ BSTR AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAccessInfo )( 
            DispZedContainer * This,
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNbAccesses )( 
            DispZedContainer * This,
            /* [retval][out] */ LONG *Count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAccess )( 
            DispZedContainer * This,
            /* [in] */ LONG Index,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FormatMessage )( 
            DispZedContainer * This,
            /* [in] */ HRESULT hrCode,
            /* [retval][out] */ BSTR *Message);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetProgressObject )( 
            DispZedContainer * This,
            /* [in] */ IZedContainerProgress *Progress);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessPassword2 )( 
            DispZedContainer * This,
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessCertificate2 )( 
            DispZedContainer * This,
            /* [in] */ BSTR CerFile,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessIndirect )( 
            DispZedContainer * This,
            /* [in] */ BSTR Path,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAccessInfo2 )( 
            DispZedContainer * This,
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate,
            /* [out] */ LONG *Role,
            /* [out] */ LONG *Attributes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeAccessRole )( 
            DispZedContainer * This,
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Role);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeAccessAttributes )( 
            DispZedContainer * This,
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Attributes);
        
        END_INTERFACE
    } DispZedContainerVtbl;

    interface DispZedContainer
    {
        CONST_VTBL struct DispZedContainerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DispZedContainer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define DispZedContainer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define DispZedContainer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define DispZedContainer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define DispZedContainer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define DispZedContainer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define DispZedContainer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define DispZedContainer_CreateContainer(This,Name,ReplaceIfExists)	\
    ( (This)->lpVtbl -> CreateContainer(This,Name,ReplaceIfExists) ) 

#define DispZedContainer_OpenContainer(This,Name,Flags,CredentialName,CredentialSecret)	\
    ( (This)->lpVtbl -> OpenContainer(This,Name,Flags,CredentialName,CredentialSecret) ) 

#define DispZedContainer_CloseContainer(This)	\
    ( (This)->lpVtbl -> CloseContainer(This) ) 

#define DispZedContainer_MoveFile(This,ExistingFilename,NewFileName,ReplaceIfExists)	\
    ( (This)->lpVtbl -> MoveFile(This,ExistingFilename,NewFileName,ReplaceIfExists) ) 

#define DispZedContainer_DeleteFile(This,ExistingFilename)	\
    ( (This)->lpVtbl -> DeleteFile(This,ExistingFilename) ) 

#define DispZedContainer_CreateFolder(This,NewFolderName)	\
    ( (This)->lpVtbl -> CreateFolder(This,NewFolderName) ) 

#define DispZedContainer_AddFile(This,ExistingFilename,NewFileName,ReplaceIfExists)	\
    ( (This)->lpVtbl -> AddFile(This,ExistingFilename,NewFileName,ReplaceIfExists) ) 

#define DispZedContainer_ExtractFile(This,ExistingFilename,NewFileName,ReplaceIfExists)	\
    ( (This)->lpVtbl -> ExtractFile(This,ExistingFilename,NewFileName,ReplaceIfExists) ) 

#define DispZedContainer_GetFileAttributes(This,ExistingFilename,Attributes)	\
    ( (This)->lpVtbl -> GetFileAttributes(This,ExistingFilename,Attributes) ) 

#define DispZedContainer_GetFileAttributesEx(This,ExistingFilename,Attributes,CreationTime,LastWriteTime,LastAccessTime,FileSize)	\
    ( (This)->lpVtbl -> GetFileAttributesEx(This,ExistingFilename,Attributes,CreationTime,LastWriteTime,LastAccessTime,FileSize) ) 

#define DispZedContainer_FindFirst(This,SearchFilename,FindContext,Attributes,Name)	\
    ( (This)->lpVtbl -> FindFirst(This,SearchFilename,FindContext,Attributes,Name) ) 

#define DispZedContainer_FindNext(This,FindContext,Attributes,Name)	\
    ( (This)->lpVtbl -> FindNext(This,FindContext,Attributes,Name) ) 

#define DispZedContainer_FindClose(This,FindContext)	\
    ( (This)->lpVtbl -> FindClose(This,FindContext) ) 

#define DispZedContainer_AddAccessPassword(This,UserId,Password,AccessId)	\
    ( (This)->lpVtbl -> AddAccessPassword(This,UserId,Password,AccessId) ) 

#define DispZedContainer_AddAccessCertificate(This,CerFile,AccessId)	\
    ( (This)->lpVtbl -> AddAccessCertificate(This,CerFile,AccessId) ) 

#define DispZedContainer_RemoveAccess(This,AccessId)	\
    ( (This)->lpVtbl -> RemoveAccess(This,AccessId) ) 

#define DispZedContainer_GetAccessInfo(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate)	\
    ( (This)->lpVtbl -> GetAccessInfo(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate) ) 

#define DispZedContainer_GetNbAccesses(This,Count)	\
    ( (This)->lpVtbl -> GetNbAccesses(This,Count) ) 

#define DispZedContainer_GetAccess(This,Index,AccessId)	\
    ( (This)->lpVtbl -> GetAccess(This,Index,AccessId) ) 

#define DispZedContainer_FormatMessage(This,hrCode,Message)	\
    ( (This)->lpVtbl -> FormatMessage(This,hrCode,Message) ) 

#define DispZedContainer_SetProgressObject(This,Progress)	\
    ( (This)->lpVtbl -> SetProgressObject(This,Progress) ) 

#define DispZedContainer_AddAccessPassword2(This,UserId,Password,Role,Attributes,AccessId)	\
    ( (This)->lpVtbl -> AddAccessPassword2(This,UserId,Password,Role,Attributes,AccessId) ) 

#define DispZedContainer_AddAccessCertificate2(This,CerFile,Role,Attributes,AccessId)	\
    ( (This)->lpVtbl -> AddAccessCertificate2(This,CerFile,Role,Attributes,AccessId) ) 

#define DispZedContainer_AddAccessIndirect(This,Path,Attributes,AccessId)	\
    ( (This)->lpVtbl -> AddAccessIndirect(This,Path,Attributes,AccessId) ) 

#define DispZedContainer_GetAccessInfo2(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate,Role,Attributes)	\
    ( (This)->lpVtbl -> GetAccessInfo2(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate,Role,Attributes) ) 

#define DispZedContainer_ChangeAccessRole(This,AccessId,Role)	\
    ( (This)->lpVtbl -> ChangeAccessRole(This,AccessId,Role) ) 

#define DispZedContainer_ChangeAccessAttributes(This,AccessId,Attributes)	\
    ( (This)->lpVtbl -> ChangeAccessAttributes(This,AccessId,Attributes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __DispZedContainer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_ZedX_0000_0002 */
/* [local] */ 

/*------------------------------------------------------------------------------
IZedContainer
------------------------------------------------------------------------------*/


extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0002_v0_0_s_ifspec;

#ifndef __IZedContainer_INTERFACE_DEFINED__
#define __IZedContainer_INTERFACE_DEFINED__

/* interface IZedContainer */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IZedContainer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("000000F0-8804-4CA8-8868-36F59DEFD14D")
    IZedContainer : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateContainer( 
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenContainer( 
            /* [in] */ BSTR Name,
            /* [in] */ ULONG Flags,
            /* [in] */ BSTR CredentialName,
            /* [in] */ BSTR CredentialSecret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseContainer( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveFile( 
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteFile( 
            /* [in] */ BSTR ExistingFilename) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateFolder( 
            /* [in] */ BSTR NewFolderName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddFile( 
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtractFile( 
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileAttributes( 
            /* [in] */ BSTR ExistingFilename,
            /* [retval][out] */ LONG *Attributes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileAttributesEx( 
            /* [in] */ BSTR ExistingFilename,
            /* [out] */ LONG *Attributes,
            /* [out] */ DATE *CreationTime,
            /* [out] */ DATE *LastWriteTime,
            /* [out] */ DATE *LastAccessTime,
            /* [out] */ LONGLONG *FileSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindFirst( 
            /* [in] */ BSTR SearchFilename,
            /* [out] */ LONG *FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindNext( 
            /* [in] */ LONG FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindClose( 
            /* [in] */ LONG FindContext) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessPassword( 
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessCertificate( 
            /* [in] */ BSTR CerFile,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveAccess( 
            /* [in] */ BSTR AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAccessInfo( 
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNbAccesses( 
            /* [retval][out] */ LONG *Count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAccess( 
            /* [in] */ LONG Index,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FormatMessage( 
            /* [in] */ HRESULT hrCode,
            /* [retval][out] */ BSTR *Message) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetProgressObject( 
            /* [in] */ IZedContainerProgress *Progress) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IZedContainerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZedContainer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZedContainer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZedContainer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateContainer )( 
            IZedContainer * This,
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenContainer )( 
            IZedContainer * This,
            /* [in] */ BSTR Name,
            /* [in] */ ULONG Flags,
            /* [in] */ BSTR CredentialName,
            /* [in] */ BSTR CredentialSecret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseContainer )( 
            IZedContainer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveFile )( 
            IZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteFile )( 
            IZedContainer * This,
            /* [in] */ BSTR ExistingFilename);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateFolder )( 
            IZedContainer * This,
            /* [in] */ BSTR NewFolderName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddFile )( 
            IZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtractFile )( 
            IZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [in] */ BSTR NewFileName,
            /* [in] */ VARIANT_BOOL ReplaceIfExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileAttributes )( 
            IZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [retval][out] */ LONG *Attributes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileAttributesEx )( 
            IZedContainer * This,
            /* [in] */ BSTR ExistingFilename,
            /* [out] */ LONG *Attributes,
            /* [out] */ DATE *CreationTime,
            /* [out] */ DATE *LastWriteTime,
            /* [out] */ DATE *LastAccessTime,
            /* [out] */ LONGLONG *FileSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindFirst )( 
            IZedContainer * This,
            /* [in] */ BSTR SearchFilename,
            /* [out] */ LONG *FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindNext )( 
            IZedContainer * This,
            /* [in] */ LONG FindContext,
            /* [out] */ LONG *Attributes,
            /* [retval][out] */ BSTR *Name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindClose )( 
            IZedContainer * This,
            /* [in] */ LONG FindContext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessPassword )( 
            IZedContainer * This,
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessCertificate )( 
            IZedContainer * This,
            /* [in] */ BSTR CerFile,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAccess )( 
            IZedContainer * This,
            /* [in] */ BSTR AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAccessInfo )( 
            IZedContainer * This,
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNbAccesses )( 
            IZedContainer * This,
            /* [retval][out] */ LONG *Count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAccess )( 
            IZedContainer * This,
            /* [in] */ LONG Index,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FormatMessage )( 
            IZedContainer * This,
            /* [in] */ HRESULT hrCode,
            /* [retval][out] */ BSTR *Message);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetProgressObject )( 
            IZedContainer * This,
            /* [in] */ IZedContainerProgress *Progress);
        
        END_INTERFACE
    } IZedContainerVtbl;

    interface IZedContainer
    {
        CONST_VTBL struct IZedContainerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZedContainer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IZedContainer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IZedContainer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IZedContainer_CreateContainer(This,Name,ReplaceIfExists)	\
    ( (This)->lpVtbl -> CreateContainer(This,Name,ReplaceIfExists) ) 

#define IZedContainer_OpenContainer(This,Name,Flags,CredentialName,CredentialSecret)	\
    ( (This)->lpVtbl -> OpenContainer(This,Name,Flags,CredentialName,CredentialSecret) ) 

#define IZedContainer_CloseContainer(This)	\
    ( (This)->lpVtbl -> CloseContainer(This) ) 

#define IZedContainer_MoveFile(This,ExistingFilename,NewFileName,ReplaceIfExists)	\
    ( (This)->lpVtbl -> MoveFile(This,ExistingFilename,NewFileName,ReplaceIfExists) ) 

#define IZedContainer_DeleteFile(This,ExistingFilename)	\
    ( (This)->lpVtbl -> DeleteFile(This,ExistingFilename) ) 

#define IZedContainer_CreateFolder(This,NewFolderName)	\
    ( (This)->lpVtbl -> CreateFolder(This,NewFolderName) ) 

#define IZedContainer_AddFile(This,ExistingFilename,NewFileName,ReplaceIfExists)	\
    ( (This)->lpVtbl -> AddFile(This,ExistingFilename,NewFileName,ReplaceIfExists) ) 

#define IZedContainer_ExtractFile(This,ExistingFilename,NewFileName,ReplaceIfExists)	\
    ( (This)->lpVtbl -> ExtractFile(This,ExistingFilename,NewFileName,ReplaceIfExists) ) 

#define IZedContainer_GetFileAttributes(This,ExistingFilename,Attributes)	\
    ( (This)->lpVtbl -> GetFileAttributes(This,ExistingFilename,Attributes) ) 

#define IZedContainer_GetFileAttributesEx(This,ExistingFilename,Attributes,CreationTime,LastWriteTime,LastAccessTime,FileSize)	\
    ( (This)->lpVtbl -> GetFileAttributesEx(This,ExistingFilename,Attributes,CreationTime,LastWriteTime,LastAccessTime,FileSize) ) 

#define IZedContainer_FindFirst(This,SearchFilename,FindContext,Attributes,Name)	\
    ( (This)->lpVtbl -> FindFirst(This,SearchFilename,FindContext,Attributes,Name) ) 

#define IZedContainer_FindNext(This,FindContext,Attributes,Name)	\
    ( (This)->lpVtbl -> FindNext(This,FindContext,Attributes,Name) ) 

#define IZedContainer_FindClose(This,FindContext)	\
    ( (This)->lpVtbl -> FindClose(This,FindContext) ) 

#define IZedContainer_AddAccessPassword(This,UserId,Password,AccessId)	\
    ( (This)->lpVtbl -> AddAccessPassword(This,UserId,Password,AccessId) ) 

#define IZedContainer_AddAccessCertificate(This,CerFile,AccessId)	\
    ( (This)->lpVtbl -> AddAccessCertificate(This,CerFile,AccessId) ) 

#define IZedContainer_RemoveAccess(This,AccessId)	\
    ( (This)->lpVtbl -> RemoveAccess(This,AccessId) ) 

#define IZedContainer_GetAccessInfo(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate)	\
    ( (This)->lpVtbl -> GetAccessInfo(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate) ) 

#define IZedContainer_GetNbAccesses(This,Count)	\
    ( (This)->lpVtbl -> GetNbAccesses(This,Count) ) 

#define IZedContainer_GetAccess(This,Index,AccessId)	\
    ( (This)->lpVtbl -> GetAccess(This,Index,AccessId) ) 

#define IZedContainer_FormatMessage(This,hrCode,Message)	\
    ( (This)->lpVtbl -> FormatMessage(This,hrCode,Message) ) 

#define IZedContainer_SetProgressObject(This,Progress)	\
    ( (This)->lpVtbl -> SetProgressObject(This,Progress) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IZedContainer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_ZedX_0000_0003 */
/* [local] */ 

/*------------------------------------------------------------------------------
IZedContainer2
------------------------------------------------------------------------------*/


extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0003_v0_0_s_ifspec;

#ifndef __IZedContainer2_INTERFACE_DEFINED__
#define __IZedContainer2_INTERFACE_DEFINED__

/* interface IZedContainer2 */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IZedContainer2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("00001036-8804-4CA8-8868-36F59DEFD14D")
    IZedContainer2 : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessPassword2( 
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessCertificate2( 
            /* [in] */ BSTR CerFile,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddAccessIndirect( 
            /* [in] */ BSTR Path,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAccessInfo2( 
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate,
            /* [out] */ LONG *Role,
            /* [out] */ LONG *Attributes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeAccessRole( 
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Role) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeAccessAttributes( 
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Attributes) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IZedContainer2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZedContainer2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZedContainer2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZedContainer2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessPassword2 )( 
            IZedContainer2 * This,
            /* [in] */ BSTR UserId,
            /* [in] */ BSTR Password,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessCertificate2 )( 
            IZedContainer2 * This,
            /* [in] */ BSTR CerFile,
            /* [in] */ LONG Role,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddAccessIndirect )( 
            IZedContainer2 * This,
            /* [in] */ BSTR Path,
            /* [in] */ LONG Attributes,
            /* [retval][out] */ BSTR *AccessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAccessInfo2 )( 
            IZedContainer2 * This,
            /* [in] */ BSTR AccessId,
            /* [out] */ LONG *AccessType,
            /* [out] */ BSTR *Label,
            /* [out] */ BSTR *IndirectAccessFileName,
            /* [out] */ BSTR *BinaryCertificate,
            /* [out] */ LONG *Role,
            /* [out] */ LONG *Attributes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeAccessRole )( 
            IZedContainer2 * This,
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Role);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeAccessAttributes )( 
            IZedContainer2 * This,
            /* [in] */ BSTR AccessId,
            /* [in] */ LONG Attributes);
        
        END_INTERFACE
    } IZedContainer2Vtbl;

    interface IZedContainer2
    {
        CONST_VTBL struct IZedContainer2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZedContainer2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IZedContainer2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IZedContainer2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IZedContainer2_AddAccessPassword2(This,UserId,Password,Role,Attributes,AccessId)	\
    ( (This)->lpVtbl -> AddAccessPassword2(This,UserId,Password,Role,Attributes,AccessId) ) 

#define IZedContainer2_AddAccessCertificate2(This,CerFile,Role,Attributes,AccessId)	\
    ( (This)->lpVtbl -> AddAccessCertificate2(This,CerFile,Role,Attributes,AccessId) ) 

#define IZedContainer2_AddAccessIndirect(This,Path,Attributes,AccessId)	\
    ( (This)->lpVtbl -> AddAccessIndirect(This,Path,Attributes,AccessId) ) 

#define IZedContainer2_GetAccessInfo2(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate,Role,Attributes)	\
    ( (This)->lpVtbl -> GetAccessInfo2(This,AccessId,AccessType,Label,IndirectAccessFileName,BinaryCertificate,Role,Attributes) ) 

#define IZedContainer2_ChangeAccessRole(This,AccessId,Role)	\
    ( (This)->lpVtbl -> ChangeAccessRole(This,AccessId,Role) ) 

#define IZedContainer2_ChangeAccessAttributes(This,AccessId,Attributes)	\
    ( (This)->lpVtbl -> ChangeAccessAttributes(This,AccessId,Attributes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IZedContainer2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_ZedX_0000_0004 */
/* [local] */ 

/*------------------------------------------------------------------------------
Access Enums
------------------------------------------------------------------------------*/
typedef /* [helpstring][v1_enum] */ 
enum ZEDXAPI_ROLE_FLAGS
    {
        ZEDXAPI_ROLE_NONE	= 0,
        ZEDXAPI_ROLE_ADMIN	= 0x1,
        ZEDXAPI_ROLE_RECOVERY	= 0x2,
        ZEDXAPI_ROLE_MANDATORY	= 0x4
    } 	ZEDXAPI_ROLE_FLAGS;

typedef /* [helpstring][v1_enum] */ 
enum ZEDXAPI_ACCESS_FLAGS
    {
        ZEDXAPI_ATTRIBUTE_NONE	= 0,
        ZEDXAPI_ATTRIBUTE_IGNOREADMIN	= 0x1000000
    } 	ZEDXAPI_ACCESS_FLAGS;



extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ZedX_0000_0004_v0_0_s_ifspec;


#ifndef __ZedX_LIBRARY_DEFINED__
#define __ZedX_LIBRARY_DEFINED__

/* library ZedX */
/* [helpstring][version][uuid] */ 







EXTERN_C const IID LIBID_ZedX;

EXTERN_C const CLSID CLSID_ZedContainer;

#ifdef __cplusplus

class DECLSPEC_UUID("000000F4-8804-4CA8-8868-36F59DEFD14D")
ZedContainer;
#endif
#endif /* __ZedX_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


