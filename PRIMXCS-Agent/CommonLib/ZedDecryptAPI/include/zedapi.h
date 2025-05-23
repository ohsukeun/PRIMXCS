/*
ZED>API - Version 2023    --Mark:$ZEDAPIVERSION$

Product(s)  ZONECENTRAL, ZEDMAIL, ZED! FULL EDITION
Platforms   Windows 7, 8.1, 10
File:        zedapi.h

Usage:
-----
    - #include "this header"
    - dynamically load the functions from the zedx.dll or link with
      the zedapi.lib library
    - the zedx.dll bridge must always be installed by your application
    - the API only works with an installed run-time (see product list
        above). If the run-time is a Limited Edition, features are also
        limited.

Quick Reference:
---------------
    ZEDAPI_OpenContainer
        To open an existing container (with credentials)
    ZEDAPI_CreateContainer
        To create a new container (credentials to be supplied later)
    ZEDAPI_CloseHandle
        To close an opened container


    ZEDAPI_MoveFile
        To move or rename a file or a folder in a container
    ZEDAPI_DeleteFile
        To delete a file or en ampty folder in a container
    ZEDAPI_CreateFolder
        To create an (empty) folder in a container
    ZEDAPI_AddFile
        To add a file in a container
    ZEDAPI_ExtractFile
        To extract a file from a container
    ZEDAPI_GetFileAttributes
        To get a file or folder atrributes
    ZEDAPI_GetFileAttributesEx
        To get a file or folder attributes and information
    ZEDAPI_FindFirstFile
        To start a (non-recursive) search inside a given folder in a 
        container
    ZEDAPI_FindNextFile
        To continue a previous search
    ZEDAPI_FindClose
        To terminate a previous search


    ZEDAPI_AddAccessPassword
        To add a new password access in a container
    ZEDAPI_AddAccessPassword2
        To add a new password access in a container with role and attributes
    ZEDAPI_AddAccessCertificate
        To add a new certificate access in a container
    ZEDAPI_AddAccessCertificate2
        To add a new certificate access in a container with role and attributes
    ZEDAPI_AddAccessIndirect
        To add a new indirect access in a container
    ZEDAPI_RemoveAccess
        To remove an existing access from a container
    ZEDAPI_GetAccesses
        To get the list of existing accesses
    ZEDAPI_GetAccessInfo
        To get information about a given access
    ZEDAPI_GetAccessInfo2
        To get information about a given access
    ZEDAPI_ChangeAccessRole
    To change the role of a given access
    ZEDAPI_ChangeAccessAttributes
        To change the attributes of a given access


    ZEDAPI_FormatMessage
        To get the message text of a given error code

Notes:
-----
    Except when mentionned, relative paths, or paths with "." or ".." 
        are NOT supported.
    All file-system paths must be absolute, and without any upward expression.
    All zed-items paths must be absolute WITHIN the zed file, and without
        any upward expression (like \test\..\test2)

Copyright Prim'X Technologies SA 2004, 2014.
------------------------------------------------------------------------------*/

#ifndef __ZEDAPI_H__
#define __ZEDAPI_H__

#ifdef __cplusplus
extern "C"{
#endif 

#ifndef ZEDAPICALL
#define ZEDAPICALL __declspec(dllimport) __stdcall
#endif

#ifndef __ZEDAPIIMPL__
typedef struct {int _dontuse;} * ZEDAPI_HANDLE;
typedef struct {int _dontuse;} * ZEDAPI_FIND_HANDLE;
#endif

#define ZEDAPI_INVALID_HANDLE (ZEDAPI_HANDLE)0
#define ZEDAPI_INVALID_FIND_HANDLE (ZEDAPI_FIND_HANDLE)0

/*==============================================================================

RETURN CODES

==============================================================================*/

/* S_OK
Operation Successfull
Defined in <winerror.h>
*/

/* E_INVALIDARG
One or more arguments are invalid
Defined in <winerror.h>
*/

/* E_HANDLE
Invalid handle
Here: invalid container handle / Invalid search handle
Defined in <winerror.h>
*/

/* E_DENIED
Operation denied
*/
#ifndef E_DENIED
#define E_DENIED __HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)
#endif

/* E_TOO_SMALL
Insufficient buffer space
*/
#ifndef E_TOO_SMALL
#define E_TOO_SMALL __HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
#endif

/* E_FILE_NOT_FOUND
The specified file does not exist
*/
#ifndef E_FILE_NOT_FOUND
#define E_FILE_NOT_FOUND __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
#endif

/* E_FILE_EXISTS
The specified file already exists
*/
#ifndef E_FILE_EXISTS
#define E_FILE_EXISTS __HRESULT_FROM_WIN32(ERROR_FILE_EXISTS)
#endif

/* E_BAD_PATHNAME
The specified pathname is illegal or does not exists
*/
#ifndef E_BAD_PATHNAME
#define E_BAD_PATHNAME __HRESULT_FROM_WIN32(ERROR_BAD_PATHNAME)
#endif

/* E_DIR_NOT_EMPTY
The specified directory is not empty
*/
#ifndef E_DIR_NOT_EMPTY
#define E_DIR_NOT_EMPTY __HRESULT_FROM_WIN32(ERROR_DIR_NOT_EMPTY)
#endif

/* E_NO_MORE_FILES
No more file found in search
*/
#ifndef E_NO_MORE_FILES
#define E_NO_MORE_FILES __HRESULT_FROM_WIN32(ERROR_NO_MORE_FILES)
#endif

/* E_NOT_FOUND
Item not found
*/
#ifndef E_NOT_FOUND
#define E_NOT_FOUND __HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
#endif

/* For other return codes, use the ZEDAPI_FormatMessage() function 

Any HRESULT code with a FACILITY code value 0x7x is a Prim'X error code:
    hr=807xYYYY, where 7x is the FACILITY code, and YYYY the error value

*/

/*------------------------------------------------------------------------------
Function:   ZEDAPI_FormatMessage
            The FormatMessage function returns the message text for a given
            error code. 
hrCode [in]:
            Error code
pwzMessage [out]:
            Caller-allocated buffer to receive the message text. The value 
            pointed by the pcch parameter must contain the allocated length, 
            in characters, including the terminating NULL.
            This parameter can be NULL. If so, the input value of pcch is
            ignored, and its output value is filled with the length of the
            message (in chars, with the last NULL).
pcch [in, out]:
            On input, it contains the allocated length, in characters, 
            of the buffer pointed by the pwzMessage parameter, 
            including the terminating NULL.
            On output, it contains the actual length, in characters, of the
            message text copied to the buffer pointed by the pwzMessage
            parameter (including the terminating NULL).
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_TOO_SMALL: buffer too small to contain the message
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_FormatMessage(
    HRESULT hrCode,
    WCHAR * pwzMessage,
    unsigned int * pcch );

/*==============================================================================

CONTAINERS MANAGEMENT FUNCTIONS

==============================================================================*/

/*------------------------------------------------------------------------------
Flags
------------------------------------------------------------------------------*/

/* Credential flags */
/* Use the built-in graphical GUI to get the credential */
#define ZEDAPI_CRED_UI       0x10000000
/* Use the current run-time context to get the credential
   (apply to ZoneCentral & ZoneExpress run-time only) */
#define ZEDAPI_CRED_RT       0x20000000
/* Use a (given) passsword credential */
#define ZEDAPI_CRED_PWD      0x00000001
/* Use a (given) keyfile credential */
#define ZEDAPI_CRED_PFX      0x00000002
/* Open container in read-only mode */
#define ZEDAPI_OPEN_READONLY 0x01000000
/*------------------------------------------------------------------------------
Function:   ZEDAPI_OpenContainer
            The OpenContainer function opens an existing encrypted container, 
            with the specified credential.
pcwzContainerName [in]:
            Pointer to a null-terminated string that names an existing 
            encrypted container.
dwFlags [in]:
            This parameter can be one or more of the following values:
            ZEDAPI_OPEN_READONLY
                Open the container in read-only mode 
            ZEDAPI_CRED_RT
                Use the current run-time context
            ZEDAPI_CRED_UI
                Use the built-in graphical GUI to get the credential
                Must be set with ZEDAPI_CRED_RT (ignored if not)
            ZEDAPI_CRED_PWD
                The supplied credential is a user+password credential
            ZEDAPI_CRED_PFX
                The supplied credential is a keyfile+password credential

            1/ The given credential, if any (ZEDAPI_CRED_PWD or  
            ZEDAPI_CRED_PFX) is tried.
            2/ If unsuccessfull, or if there is no given credential, and if
            ZEDAPI_CRED_RT is present, the current context is tried, with :
                - available keys (of any kind, from any store) in the product
                  run-time context. This is only for ZONECENTRAL run-time
                - available, opened, and loaded PKCS#11 tokens
                  (the token must have been opened either par the product
                  run-time or by another application, if the Pkcs#11 middleware
                  enables a single Pin-On among applications)
                - available CSP containers : Beware, the CSP container may
                  display its own graphical interface
            3/ If unsuccessfull, and if ZEDAPI_CRED_UI is present, the run-time
            UI is displayed to get a credential.

            It is not possible to open a container with a Pkcs#11 token by
        suppliying its PIN code.
            It is not possible to open a container with a CSP container
            without any UI if the CSP has decided to show its UI.

pcwzCredName [in]:
            Name of the credential to use.
            With ZEDAPI_CRED_PWD, it must contain the user-id.
            With ZEDAPI_CRED_PFX, it must contain the full path name of a .pfx
                (.p12) file
            Not used on other cases.
pcwzCredSecret [in]:
            Secret of the credential to use.
            With ZEDAPI_CRED_PWD, it must contain the password value.
            With ZEDAPI_CRED_PFX, it must contain the access code to the
                given key-file.
            Not used on other cases.
pzedhdl [out]:
            Handle to the opened encrypted container. This handle must be
            supplied in other functions.
            This handle must be closed after use with ZEDAPI_CloseHandle().
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_FILE_NOT_FOUND: the pcwzContainerName does not exists
            E_DENIED: access denied to the container
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_OpenContainer(
    const WCHAR * pcwzContainerName, 
    DWORD dwFlags, 
    const WCHAR * pcwzCredName, 
    const WCHAR * pcwzCredSecret,
    ZEDAPI_HANDLE * pzedhdl );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_CreateContainer
            The CreateContainer function creates a new encrypted container.
            The container is created with NO access, except for the
            mandatory accesses (P131)
pcwzNewContainerName [in]:
            Pointer to a null-terminated string that names a new encrypted 
            container.
dwFlags [in]:
            This parameter can be one or more of the following values:
            ZEDAPI_REPLACE_EXISTING:
                If a file named pcwzNewContainerName exists, the function 
                replaces it with a new container.
pzedhdl [out]:
            Handle to the opened encrypted container. This handle must be
            supplied in other functions.
            This handle must be closed after use with ZEDAPI_CloseHandle().
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_FILE_EXISTS: the pcwzNewContainerName file already exists
            E_DENIED: access denied to the container
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_CreateContainer(
    const WCHAR * pcwzNewContainerName,
    DWORD dwFlags,
    ZEDAPI_HANDLE * pzedhdl );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_CloseHandle
            The CloseHandle function closes a previously opened container
            handle (obtained with OpenContainer or CreateContainer)
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_CloseHandle(
    ZEDAPI_HANDLE zedhdl );



/*==============================================================================

FILES MANAGEMENT FUNCTIONS

==============================================================================*/

/*------------------------------------------------------------------------------
Flags
------------------------------------------------------------------------------*/
/* Replace a target file if it already exists */
#define ZEDAPI_REPLACE_EXISTING     1

/*------------------------------------------------------------------------------
Attributes
------------------------------------------------------------------------------*/
/* Attributes */
#define ZEDAPI_FILE_ATTRIBUTE_DIRECTORY     (FILE_ATTRIBUTE_DIRECTORY)

/* Extended attributes */
typedef WIN32_FILE_ATTRIBUTE_DATA ZEDAPI_FILE_ATTRIBUTE_DATA;

/*------------------------------------------------------------------------------
Search results
------------------------------------------------------------------------------*/
/* Note: the cAlternateFileName field is never filled and is not used */
typedef WIN32_FIND_DATAW ZEDAPI_FIND_DATA;

/*------------------------------------------------------------------------------
Function:   ZEDAPI_PROGRESS_ROUTINE
            Callback Progress Routine
            Any Error code (including E_CANCEL) return will STOP the current 
            operation, except for the first call (Size=global size, Done=0),
            where the return code is ignored.
------------------------------------------------------------------------------*/
typedef HRESULT (__stdcall * ZEDAPI_ProgressRoutine_Ptr)(
    void * pvData,
    unsigned __int64 ui64Size,
    unsigned __int64 ui64Done
);
#define ZEDAPI_PROGRESS_ROUTINE ZEDAPI_ProgressRoutine_Ptr

/*------------------------------------------------------------------------------
Function:   ZEDAPI_MoveFile
            The MoveFile function moves an existing file or a directory, 
            including its children, inside an encrypted container.
            Both Source and Destination are on the SAME container.
            This function can be used to rename a file or a folder.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzExistingFileName [in]:
            Pointer to a null-terminated string that names an existing 
            file or folder. The name must be absolute within the encrypted
            container.
pcwzNewFileName [in]:
            Pointer to a null-terminated string that specifies the new 
            name of a file or folder. The new name must not already exist. 
            The new name must be absolute within the encrypted container.
dwFlags [in]:
            This parameter can be one or more of the following values:
            ZEDAPI_REPLACE_EXISTING:
                If a file named pcwzNewFileName exists, the function replaces 
                it with the pcwzExistingFileName file. 
                This value cannot be used if pcwzExistingFileName names a 
                directory.
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_NOT_FOUND: the pcwzExistingFileName does not exists
            E_FILE_EXISTS: the pcwzNewFileName file already exists
            E_BAD_PATHNAME: the path specified in pcwzNewFileName 
                is incorrect or does not exist
            E_DENIED: access denied to the container
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_MoveFile(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzExistingFileName, 
    const WCHAR * pcwzNewFileName,
    DWORD dwFlags );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_DeleteFile
            The DeleteFile function deletes an existing file or an empty folder
            from an encrypted container.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzExistingFileName [in]:
            Pointer to a null-terminated string that names an existing 
            file or folder. The name must be absolute within the encrypted
            container.
            If the name is a folder, the folder must be empty.
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_NOT_FOUND: the pcwzExistingFileName does not exists
            E_DIR_NOT_EMPTY: the pcwzExistingFileName is a non-empty folder
            E_DENIED: access denied to the container
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_DeleteFile(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzExistingFileName );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_CreateFolder
            The CreateFolder function creates a new folder or subfolder in an 
            encrypted containter. The location (subfolder) where the new folder
            is to be created must already exist.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzNewFolderName [in]:
            Pointer to a null-terminated string that names the new name of the
            folder. The new name must not already exist. 
            The new name must be absolute within the encrypted container.
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_EXISTS: the pcwzNewFolderName folder already exists
            E_BAD_PATHNAME: the path specified in pcwzNewFolderName
                is incorrect or does not exist
            E_DENIED: access denied to the container
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_CreateFolder(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzNewFolderName );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_AddFile
            The AddFile function copies an external file inside an encrypted 
            container.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzExistingFileName [in]:
            Pointer to a null-terminated string that names an existing 
            EXTERNAL file.
            This parameter cannot be a folder.
pcwzNewFileName [in]:
            Pointer to a null-terminated string that specifies the new 
            name of the file. The new name must not already exist. 
            The new name must be absolute within the encrypted container.
dwFlags [in]:
            This parameter can be one or more of the following values:
            ZEDAPI_REPLACE_EXISTING:
                If a file named pcwzNewFileName exists, the function replaces 
                it with the pcwzExistingFileNamefile. 
pfnProgressRoutine [in]:
            Pointer to a ZEDAPI_ProgressRoutine callback function that is 
            called each time another portion of the file has been processed. 
            The callback function can be useful if you provide a user interface 
            that displays the progress of the operation. 
            This parameter can be NULL. 
pvData [in]:
            Argument to be passed to the pfnProgressRoutine callback function.
            This parameter can be NULL.
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_NOT_FOUND: the pcwzExistingFileName does not exists
            E_FILE_EXISTS: the pcwzNewFileName file already exists
            E_BAD_PATHNAME: the path specified in pcwzNewFileName 
                is incorrect or does not exist
            E_DENIED: access denied to the container
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_AddFile(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzExistingFileName, 
    const WCHAR * pcwzNewFileName,
    DWORD dwFlags, 
    ZEDAPI_PROGRESS_ROUTINE pfnProgressRoutine, 
    void * pvData );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_ExtractFile
            The ExtractFile function copies a file from an encrypted container
            to an EXTERNAL file.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzExistingFileName [in]:
            Pointer to a null-terminated string that names an existing 
            file inside the container.
            The name must be absolute within the encrypted container.
            This parameter cannot be a folder.
pcwzNewFileName [in]:
            Pointer to a null-terminated string that specifies where the file 
            must be copied, with the new name of the file. 
            The new name must not already exist. 
dwFlags [in]:
            This parameter can be one or more of the following values:
            ZEDAPI_REPLACE_EXISTING:
                If a file named pcwzNewFileName exists, the function replaces 
                it with the pcwzExistingFileNamefile. 
pfnProgressRoutine [in]:
            Pointer to a ZEDAPI_ProgressRoutine callback function that is 
            called each time another portion of the file has been processed. 
            The callback function can be useful if you provide a user interface 
            that displays the progress of the operation. 
            This parameter can be NULL. 
pvData [in]:
            Argument to be passed to the pfnProgressRoutine callback function.
            This parameter can be NULL.
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_NOT_FOUND: the pcwzExistingFileName does not exists
            E_FILE_EXISTS: the pcwzNewFileName file already exists
            E_BAD_PATHNAME: the path specified in pcwzExistingFileName 
                is incorrect or does not exist
            E_DENIED: access denied to the container
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_ExtractFile(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzExistingFileName, 
    const WCHAR * pcwzNewFileName,
    DWORD dwFlags, 
    ZEDAPI_PROGRESS_ROUTINE pfnProgressRoutine, 
    void * pvData );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_GetFileAttributes
            The GetFileAttributes function retrieves a set of attributes for a 
            specified file or directory inside an encrypted container.
            This function can be used to easily determine if an item already 
            exists, or if it is a file or folder.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzExistingFileName [in]:
            Pointer to a null-terminated string that names an existing 
            file or folder inside the container.
            The name must be absolute within the encrypted container.
pdwAttributes [out]:
            The attributes can be one or more of the following values:
            ZEDAPI_FILE_ATTRIBUTE_DIRECTORY
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_NOT_FOUND: the pcwzExistingFileName does not exists
            E_BAD_PATHNAME: the path specified in pcwzExistingFileName
                is incorrect or does not exist
            E_DENIED: access denied to the container
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_GetFileAttributes(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzExistingFileName, 
    DWORD * pdwAttributes );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_GetFileAttributesEx
            The GetFileAttributesEx function retrieves attributes for a 
            specified file or directory inside an encrypted container.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzExistingFileName [in]:
            Pointer to a null-terminated string that names an existing 
            file or folder inside the container.
            The name must be absolute within the encrypted container.
pstFileInformation [out]:
            A buffer that receives the file attribute information.
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_BAD_PATHNAME: the path specified in pcwzExistingFileName
                is incorrect or does not exist
            E_DENIED: access denied to the container
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_GetFileAttributesEx(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzExistingFileName, 
    ZEDAPI_FILE_ATTRIBUTE_DATA * pstFileInformation );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_FindFirstFile
            The FindFirstFile function searches a folder for a file or 
            subfolder with a name that matches a specific name.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzSearchFileName [in]:
            A pointer to a null-terminated string that specifies a valid folder
            or path, and file name that can contain wildcard characters, 
            for example, an asterisk (*) or a question mark (?). 
phFindFile [out]:
            A search handle used in a subsequent call to FindNextFile or 
            FindClose.
            If the function fails, it returns ZEDAPI_INVALID_FIND_HANDLE
            in this parameter.
pstFindFileData [out]:
            Pointer to a caller_allocated ZEDAPI_FIND_DATA structure that 
            receives information about the first file or subfolder found,
            if any. 
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle
            E_FILE_NOT_FOUND: no file found with the search pattern.
                The phFindFile returned is ZEDAPI_INVALID_FIND_HANDLE
Note:       - unlike its win32 equivalent, this function will not return the
              special "." and ".." subfolders.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_FindFirstFile(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzSearchFileName, 
    ZEDAPI_FIND_HANDLE * phFindFile,
    ZEDAPI_FIND_DATA * pstFindFileData );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_FindNextFile
            The FindFirstFile function searches a folder for a file or 
            subfolder with a name that matches a specific name.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
hFindFile [in]:
            Search handle returned by a previous call to the FindFirstFile 
            function. 
pstFindFileData [out]:
            Pointer to a caller_allocated ZEDAPI_FIND_DATA structure that 
            receives information about the found file or subfolder. 
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle OR
                      invalid hFindFile search context handle
            E_NO_MORE_FILES: no more file found in search
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_FindNextFile(
    ZEDAPI_HANDLE zedhdl,
    ZEDAPI_FIND_HANDLE hFindFile,
    ZEDAPI_FIND_DATA * pstFindFileData );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_FindClose
            The FindClose function closes a file search handle opened by the 
            FindFirstFile
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
hFindFile [in]:
            Search handle returned by a previous call to the FindFirstFile 
            function. 
Common Return Codes:
            S_OK: operation successfull
            E_INVALIDARG: function called with an invalid parameter
            E_HANDLE: Invalid container handle OR
                             invalid hFindFile search context handle
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_FindClose(
    ZEDAPI_HANDLE zedhdl,
    ZEDAPI_FIND_HANDLE hFindFile );



/*==============================================================================

ACCESSES MANAGEMENT FUNCTIONS

==============================================================================*/

/*------------------------------------------------------------------------------
Access information
------------------------------------------------------------------------------*/
enum ZEDAPI_ACCESS_TYPE
{
    ZEDAPI_ACCESS_PASSWORD=1,       /* Password access */
    ZEDAPI_ACCESS_CERTIFICATE=2,    /* Certificate (RSA) access */
    ZEDAPI_ACCESS_INDIRECT=3        /* Indirect (access list) access */
};

/*------------------------------------------------------------------------------
Role flags
------------------------------------------------------------------------------*/
/* No specific role : basic access */
#define ZEDAPI_ROLE_NONE            0x00000000
/* Administrative access */
#define ZEDAPI_ROLE_ADMIN            0x00000001
/* Recovery access */
#define ZEDAPI_ROLE_RECOVERY            0x00000002
/* Mandatory access */
#define ZEDAPI_ROLE_MANDATORY            0x00000004

/*------------------------------------------------------------------------------
Attribute flags
------------------------------------------------------------------------------*/
/* Direct access attributes flags */
/* No specific attribute */
#define ZEDAPI_ATTRIBUTE_NONE            0x00000000

/* Indirect access attributes flags */
/* Administrative accesses found in the access file pointed by the indirect
   access must be viewed as normal accesses */
#define ZEDAPI_ATTRIBUTE_IGNOREADMIN        0x01000000

/*------------------------------------------------------------------------------
Access information structure
------------------------------------------------------------------------------*/
struct ZEDAPI_ACCESS_INFO
{
    /* Size of this struct, for compatibility check.
       Must be filled with sizeof(ZEDAPI_ACCESS_INFO) */
    unsigned int cbSize;
    /* Access GUID */
    GUID guidAccess;
    /* Access type */
    ZEDAPI_ACCESS_TYPE eType;
    /* Access label: userId for a password access
                      friendly name for a certificate
                      accesslist label for an indirect access.
       Caller allocated.
       Filled only if not NULL.
       To get the size (in characters) of this field, make a first call
       with NULL, and the cchLabel will contain the length on return.
       The cchLabel NEVER includes the terminating NULL. */
    WCHAR * pwzLabel;
    unsigned int cchLabel;
    /* Indirect File Name: only for indirect accesses. Contains the
       access-list filename (without path).
       Caller allocated.
       Filled only if not NULL and for an indirect access.
       To get the size (in characters) of this field, make a first call
       with NULL, and the cchIndirectFileName will contain the length 
       on return.
       The cchIndirectFileName NEVER includes the terminating NULL. */
    WCHAR * pwzIndirectFileName;
    unsigned int cchIndirectFileName;
    /* Certificate: only for certificate accesses. 
       Caller allocated.
       Filled only if not NULL and for an indirect access.
       To get the size (in bytes) of this field, make a first call
       with NULL, and the cbCertificate will contain the length 
       on return */
    unsigned char * pucCertificate;
    unsigned int cbCertificate;
};
/*------------------------------------------------------------------------------
Access information structure (v2)
------------------------------------------------------------------------------*/
struct ZEDAPI_ACCESS_INFO_2
{
    /* Size of this struct, for compatibility check.
       Must be filled with sizeof(ZEDAPI_ACCESS_INFO) */
    unsigned int cbSize;
    /* Access GUID */
    GUID guidAccess;
    /* Access type */
    ZEDAPI_ACCESS_TYPE eType;
    /* Access label: userId for a password access
                      friendly name for a certificate
                      accesslist label for an indirect access.
       Caller allocated.
       Filled only if not NULL.
       To get the size (in characters) of this field, make a first call
       with NULL, and the cchLabel will contain the length on return.
       The cchLabel NEVER includes the terminating NULL. */
    WCHAR * pwzLabel;
    unsigned int cchLabel;
    /* Indirect File Name: only for indirect accesses. Contains the
       access-list filename (without path).
       Caller allocated.
       Filled only if not NULL and for an indirect access.
       To get the size (in characters) of this field, make a first call
       with NULL, and the cchIndirectFileName will contain the length 
       on return.
       The cchIndirectFileName NEVER includes the terminating NULL. */
    WCHAR * pwzIndirectFileName;
    unsigned int cchIndirectFileName;
    /* Certificate: only for certificate accesses. 
       Caller allocated.
       Filled only if not NULL and for an indirect access.
       To get the size (in bytes) of this field, make a first call
       with NULL, and the cbCertificate will contain the length 
       on return */
    unsigned char * pucCertificate;
    unsigned int cbCertificate;
    /* Role of the access. Can be one or more of the following values:
       ZEDAPI_ROLE_NONE
        Basic access, no specific role.
       ZEDAPI_ROLE_ADMIN
        Administrative access.
       ZEDAPI_ROLE_RECOVERY
        Recovery access.
       ZEDAPI_ROLE_MANDATORY
        Mandatory access.
       The administrative and recovery roles cannot be set together. */
    DWORD dwRole;
    /* Attributes of the access. Can be one or more of the following values:
       ZEDAPI_ATTRIBUTE_NONE
        No specific attribute.
       ZEDAPI_ATTRIBUTE_IGNOREADMIN
        Must be set only for an indirect access.
        Accesses in the access file are viewed as basic accesses, even 
            if they are defined as administrative or recovery accesses in 
            the access file. */
    DWORD dwAttributes;
};

/*------------------------------------------------------------------------------
Function:   ZEDAPI_AddAccessPassword
            DEPRECATED. Use ZEDAPI_AddAccessPassword2.
            The AddAccessPassword function adds a new password access to the
            currently opened container. The user-id must not already
            exists, and the password must match the current password quality
            criterias defined in policies. Policy P102 is also checked
            (enable or disable password accesses for .zed container)
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzUserId [in]:
            User-id for the password access. Must not be NULL nor empty.
            Must not already exist.
pcwzPassword [in]:
            Password value for the access. Must not be NULL.
            Must not be weak.
pguidAccess [out]:
            Unique identifier for the access added. If used, the GUID is
            allocated by the caller and filled by the function if
            sucessfull. Optional (may be NULL)
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_AddAccessPassword(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzUserId, 
    const WCHAR * pcwzPassword,
    GUID * pguidAccess);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_AddAccessPassword2
            The AddAccessPassword2 function adds a new password access to the
            currently opened container with role and attributes. The user-id 
            must not already password must match the current password quality
            criterias defined in policies. Policy P102 is also checked
            (enable or disable password accesses for .zed container)
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzUserId [in]:
            User-id for the password access. Must not be NULL nor empty.
            Must not already exist.
pcwzPassword [in]:
            Password value for the access. Must not be NULL.
            Must not be weak.
dwRole [in]:
            Role of the new access. Must be one or more of the following values:
            ZEDAPI_ROLE_NONE
            Basic access, no specific role.
            ZEDAPI_ROLE_ADMIN
            Administrative access.
            ZEDAPI_ROLE_RECOVERY
            Recovery access.
            ZEDAPI_ROLE_MANDATORY
            Mandatory access.
            The administrative and recovery roles must not be set together.
dwAttributes [in]:
            Attributes of the new access. Can be one or more of the following 
            values:
            ZEDAPI_ATTRIBUTE_NONE
            No specific attribute.
pguidAccess [out]:
            Unique identifier for the access added. If used, the GUID is
            allocated by the caller and filled by the function if
            sucessfull. Optional (may be NULL)
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_AddAccessPassword2(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzUserId, 
    const WCHAR * pcwzPassword,
    DWORD dwRole,
    DWORD dwAttributes,
    GUID * pguidAccess);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_AddAccessCertificate
            DEPRECATED. Use ZEDAPI_AddAccessCertificate2.
            The AddAccessCertificate function adds a new RSA access to the
            currently opened container. 
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcucCertificate [in]:
            Byte array that contains the X509 encoded certificate. The
            certificate must be in binary format (not Base64 encoded).
            The length of the byte array is given by the cbCertificate
            parameter.
cbCertificate [in]:
            Length, in bytes, of the pcucCertificate byte array.
pguidAccess [out]:
            Unique identifier for the access added. If used, the GUID is
            allocated by the caller and filled by the function if
            sucessfull. Optional (may be NULL)
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_AddAccessCertificate(
    ZEDAPI_HANDLE zedhdl, 
    const unsigned char * pcucCertificate, 
    unsigned int cbCertificate,
    GUID * pguidAccess);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_AddAccessCertificate2
            The AddAccessCertificate function adds a new RSA access to the
            currently opened container with role and attributes. 
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcucCertificate [in]:
            Byte array that contains the X509 encoded certificate. The
            certificate must be in binary format (not Base64 encoded).
            The length of the byte array is given by the cbCertificate
            parameter.
cbCertificate [in]:
            Length, in bytes, of the pcucCertificate byte array.
dwRole [in]:
            Role of the new access. Must be one or more of the following values:
            ZEDAPI_ROLE_NONE
            Basic access, no specific role.
            ZEDAPI_ROLE_ADMIN
            Administrative access.
            ZEDAPI_ROLE_RECOVERY
            Recovery access.
            ZEDAPI_ROLE_MANDATORY
            Mandatory access.
            The administrative and recovery roles must not be set together.
dwAttributes [in]:
            Attributes of the new access. Must be :
            ZEDAPI_ATTRIBUTE_NONE
            No specific attribute.
pguidAccess [out]:
            Unique identifier for the access added. If used, the GUID is
            allocated by the caller and filled by the function if
            sucessfull. Optional (may be NULL)
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_AddAccessCertificate2(
    ZEDAPI_HANDLE zedhdl, 
    const unsigned char * pcucCertificate, 
    unsigned int cbCertificate,
    DWORD dwRole,
    DWORD dwAttributes,
    GUID * pguidAccess);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_AddAccessIndirect
            The AddAccessIndirect function adds a new indirect access to the
            currently opened container. 
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcwzAccessList [in]:
            Pointer to a null terminated string that names the path of the 
            access file to link. It can be an absolute path or just a file name.
            If a file name is provided, the file is searched in all the access 
            file locations defined in policies.
dwAttributes [in]:
            Attributes of the new indirect access. Can be one or more of the 
            following values:
            ZEDAPI_ATTRIBUTE_NONE
            No specific attribute.
            ZCAPI_ATTRIBUTE_IGNOREADMIN
            Accesses in the access file are viewed as basic accesses, even 
                if they are defined as administrative or recovery accesses in 
                the access file.
pguidAccess [out]:
            Unique identifier for the access added. If used, the GUID is
            allocated by the caller and filled by the function if
            sucessfull. Optional (may be NULL)
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_AddAccessIndirect(
    ZEDAPI_HANDLE zedhdl, 
    const WCHAR * pcwzAccessList, 
    DWORD dwAttributes,
    GUID * pguidAccess);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_RemoveAccess
            The RemoveAccess function removes an existing access in the
            currently opened container. 
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcguidAccess [in]:
            GUID for the access that must be removed.
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
            E_NOT_FOUND: the access does not exist
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_RemoveAccess(
    ZEDAPI_HANDLE zedhdl, 
    const GUID * pcguidAccess);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_GetAccessInfo
            DEPRECATED. Use ZEDAPI_GetAccessInfo2.
            The GetAccessInfo returns various information about a given
            existing access in the currently opened container
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcguidAccess [in]:
            GUID for the access for which information must be retrieved.
pstAccessInfo [in, out]:
            Information retrieved for the access.
            On input, the cbSize field MUST be filled with the size of
                the structure.
            ANY pointed subvalue must be allocated by the caller, and the 
                length of the allocated buffer must be indicated in the 
                corresponding field, either in characters or in bytes.
            To get the length of the values for the allocation, make a
                first call with NULL pointers, and the length will be
                indicated on return. Then allocate, fill the pointers values 
                in the structure, fill the allocated length, and call again.
            For character strings, the length DOES NOT include the terminating
                NULL character. On output, the returned length never includes
                the terminating NULL. The caller must add one extra character 
                for each allocation.
            If not relevant, the returned string is an empty string (it
                contains only the terminating NULL) and the returned 
                length is 0.
            If any of the pointed subvalued has a non-NULL buffer and if
                the corresponding length is too small for the value to be
                entirely filled in, the function fails with E_TOO_SMALL.
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
            E_NOT_FOUND: the access does not exist
            E_TOO_SMALL: a buffer in the supplied structure is too small
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_GetAccessInfo(
    ZEDAPI_HANDLE zedhdl, 
    const GUID * pcguidAccess,
    struct ZEDAPI_ACCESS_INFO * pstAccessInfo);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_GetAccessInfo2
            The GetAccessInfo2 returns various information about a given
            existing access in the currently opened container
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcguidAccess [in]:
            GUID for the access for which information must be retrieved.
pstAccessInfo2 [in, out]:
            Information retrieved for the access.
            On input, the cbSize field MUST be filled with the size of
                the structure.
            ANY pointed subvalue must be allocated by the caller, and the 
                length of the allocated buffer must be indicated in the 
                corresponding field, either in characters or in bytes.
            To get the length of the values for the allocation, make a
                first call with NULL pointers, and the length will be
                indicated on return. Then allocate, fill the pointers values 
                in the structure, fill the allocated length, and call again.
            For character strings, the length DOES NOT include the terminating
                NULL character. On output, the returned length never includes
                the terminating NULL. The caller must add one extra character 
                for each allocation.
            If not relevant, the returned string is an empty string (it
                contains only the terminating NULL) and the returned 
                length is 0.
            If any of the pointed subvalued has a non-NULL buffer and if
                the corresponding length is too small for the value to be
                entirely filled in, the function fails with E_TOO_SMALL.
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
            E_NOT_FOUND: the access does not exist
            E_TOO_SMALL: a buffer in the supplied structure is too small
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_GetAccessInfo2(
    ZEDAPI_HANDLE zedhdl, 
    const GUID * pcguidAccess,
    struct ZEDAPI_ACCESS_INFO_2 * pstAccessInfo2);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_GetAccesses
            The GetAccesses returns the GUIDs of all the accesses in the
            currently opened container.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
ptabGuidAccesses [out]:
            Pointer to an array of GUIDs, to be filled by the function.
            This array is allocated by the caller, and the number of
            GUID cells must be indicated on input in the pceAccesses
            parameter.
            To get the number of accesses, make a first call with the
            ptabGuidAccesses set to NULL: on return, the pceAccesses
            parameter will contain the actual number of accesses.
            Then allocate the array with ceAccesses*sizeof(GUID), and make
            a second call, with the pointed array and the number of cells.
pceAccesses [in, out]:
            On input, this parameter contains the number of cells in the 
                ptabGuidAccesses parameter.
            On output, it always contains the actual number of accesses,
        even in the case of error E_TOO_SMALL.
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
            E_TOO_SMALL: The number of cells is too small to receive all the 
            accesses.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_GetAccesses(
    ZEDAPI_HANDLE zedhdl, 
    GUID * ptabGuidAccesses,
    unsigned int * pceAccesses);

/*------------------------------------------------------------------------------
Function:   ZEDAPI_ChangeAccessRole
            The ChangeAccessRole function changes the role of a specified 
            access.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcguidAccess [in]:
            GUID for the access whose role must be changed.
            Must be a direct access (password or certificate access).
dwRole [in]:
            New Role of the access. Must be one or more of the following values:
            ZEDAPI_ROLE_NONE
            Basic access, no specific role.
            ZEDAPI_ROLE_ADMIN
            Administrative access.
            ZEDAPI_ROLE_RECOVERY
            Recovery access.
            ZEDAPI_ROLE_MANDATORY
            Mandatory access.
            The administrative and recovery roles must not be set together.
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
            E_INVALIDARG: function called with an invalid parameter
            E_NOTFOUND: the access does not exist
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_ChangeAccessRole(
    ZEDAPI_HANDLE zedhdl, 
    const GUID * pcguidAccess,
    DWORD dwRole );

/*------------------------------------------------------------------------------
Function:   ZEDAPI_ChangeAccessAttributes
            The ChangeAccessAttributes function changes the attributes of a 
            specified access.
zedhdl [in]:
            Handle of an encrypted container context, previously obtained
            with ZEDAPI_OpenContainer().
pcguidAccess [in]:
            GUID for the access whose attributes must be changed.
dwAttributes [in]:
            New Attributes of the access. Can be one or more of the following 
            values:
            ZEDAPI_ATTRIBUTE_NONE
            No specific attribute.
            ZEDAPI_ATTRIBUTE_IGNOREADMIN
            Must be set only for an indirect access.
            Accesses in the access file are viewed as basic accesses, even 
                if they are defined as administrative or recovery accesses in 
                the access file.
Common Return Codes:
            S_OK: operation successfull
            E_HANDLE: Invalid container handle
            E_NOTFOUND: the access does not exist
            E_INVALIDARG: function called with an invalid parameter
Note:
            This function is NOT available with the ZED Reader API.
------------------------------------------------------------------------------*/
extern HRESULT ZEDAPICALL ZEDAPI_ChangeAccessAttributes(
    ZEDAPI_HANDLE zedhdl, 
    const GUID * pcguidAccess,
    DWORD dwAttributes );


#ifdef __cplusplus
}
#endif


#endif    /* __ZEDAPI_H__ */

/*============================< END OF FILE >=================================*/