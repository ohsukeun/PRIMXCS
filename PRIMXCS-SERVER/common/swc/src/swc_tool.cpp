/*****************************************************************************
 *                                                                           *
 * File Name : swc_tool.cpp                                                  *
 * Theme     : tool functions                                                *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 25                     *
 *             2. swc_isnumber_str() add ''23. 07. 04                        *
 *                                                                           *
 *****************************************************************************/

/// @file swc_tool.cpp 
/// @author Park, sung soo
/// @date 2009-11-25
/// @brief tool functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include <iconv.h>
#include <ctype.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include <swc_tool.h>

#ifndef __SWC_TOOL_CPP__
#define __SWC_TOOL_CPP__

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : debug messaging function
    parameters  :
        - (int)         output option
		- (const CHAR*) comments, ...
    return      : ()
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
namespace SWC
{
SWC_BOOL swc_chmod(IN const CHAR* spath, IN INT own, IN INT grp, IN INT other)
{
    SWC_BOOL bret = TRUE;
    mode_t mode = 0;

    if(own & (1 << 0)) mode |= S_IXUSR;
    if(own & (1 << 1)) mode |= S_IWUSR;
    if(own & (1 << 2)) mode |= S_IRUSR;

    if(grp & (1 << 0)) mode |= S_IXGRP;
    if(grp & (1 << 1)) mode |= S_IWGRP;
    if(grp & (1 << 2)) mode |= S_IRGRP;

    if(other & (1 << 0)) mode |= S_IXOTH;
    if(other & (1 << 1)) mode |= S_IWOTH;
    if(other & (1 << 2)) mode |= S_IROTH;

    if(0 > chmod(spath, mode)) bret = FALSE;
    return bret;
}
SWC_BOOL swc_chmod(IN WCString spath, IN INT own, IN INT grp, IN INT other)
{
    swc_chmod(LPCSTR(spath), own, grp, other);
    return TRUE;
}
SWC_BOOL swc_fchmod(IN INT fd, IN INT own, IN INT grp, IN INT other)
{
    SWC_BOOL bret = TRUE;
    mode_t mode = 0;

    if(own & (1 << 0)) mode |= S_IXUSR;
    if(own & (1 << 1)) mode |= S_IWUSR;
    if(own & (1 << 2)) mode |= S_IRUSR;

    if(grp & (1 << 0)) mode |= S_IXGRP;
    if(grp & (1 << 1)) mode |= S_IWGRP;
    if(grp & (1 << 2)) mode |= S_IRGRP;

    if(other & (1 << 0)) mode |= S_IXOTH;
    if(other & (1 << 1)) mode |= S_IWOTH;
    if(other & (1 << 2)) mode |= S_IROTH;

    if(0 > fchmod(fd, mode)) bret = FALSE;
    return bret;
}
VOID swc_chown_chgrp(IN const CHAR* spath, IN const CHAR* owner, IN const CHAR* group)
{
    struct passwd *u_info = NULL;
    struct group  *g_info = NULL;

    if((NULL == spath) || (NULL == owner) || (NULL == group)) return;
    if(NULL == (u_info = getpwnam(owner))) return;
    if(NULL == (g_info = getgrnam(group))) return;
    chown(spath, u_info->pw_uid, g_info->gr_gid);
}
VOID swc_chown_chgrp(IN WCString spath, IN WCString owner, IN WCString group)
{
    swc_chown_chgrp(LPCSTR(spath), LPCSTR(owner), LPCSTR(group));
}
VOID swc_fchown_fchgrp(IN INT fd, IN const CHAR* owner, IN const CHAR* group)
{
    struct passwd *u_info = NULL;
    struct group  *g_info = NULL;

    if((0 >= fd) || (NULL == owner) || (NULL == group)) return;
    if(NULL == (u_info = getpwnam(owner))) return;
    if(NULL == (g_info = getgrnam(group))) return;
    fchown(fd, u_info->pw_uid, g_info->gr_gid);
}
VOID swc_fchown_fchgrp(IN INT fd, IN WCString owner, IN WCString group)
{
    swc_fchown_fchgrp(fd, LPCSTR(owner), LPCSTR(group));
}
VOID swc_chown(IN const CHAR* spath, IN const CHAR* owner)
{
    struct passwd *u_info = NULL;
    struct stat    file_info;

    if((NULL == spath) || (NULL == owner)) return;
    if(NULL == (u_info = getpwnam(owner))) return;
    memset(&file_info, 0x00, sizeof(struct stat));
    if(0 > stat(spath, &file_info)) return;
    chown(spath, u_info->pw_uid, file_info.st_gid);
}
VOID swc_chown(IN WCString spath, IN WCString owner)
{
    swc_chown(LPCSTR(spath), LPCSTR(owner));
}
VOID swc_fchown(IN INT fd, IN const CHAR* owner)
{
    struct passwd *u_info = NULL;
    struct stat    file_info;

    if((0 >= fd) || (NULL == owner)) return;
    if(NULL == (u_info = getpwnam(owner))) return;
    memset(&file_info, 0x00, sizeof(struct stat));
    if(0 > fstat(fd, &file_info)) return;
    fchown(fd, u_info->pw_uid, file_info.st_gid);
}
VOID swc_fchown(IN INT fd, IN WCString owner)
{
    swc_fchown(fd, LPCSTR(owner));
}
VOID swc_chgrp(IN const CHAR* spath, IN const CHAR* group)
{
    struct group *g_info = NULL;
    struct stat  file_info;

    if((NULL == spath) || (NULL == group)) return;
    if(NULL == (g_info = getgrnam(group))) return;
    memset(&file_info, 0x00, sizeof(struct stat));
    if(0 > stat(spath, &file_info)) return;
    chown(spath, file_info.st_uid, g_info->gr_gid);
}
VOID swc_chgrp(IN WCString spath, IN WCString group)
{
    swc_chgrp(LPCSTR(spath), LPCSTR(group));
}
VOID swc_fchgrp(IN INT fd, IN const CHAR* group)
{
    struct group *g_info = NULL;
    struct stat  file_info;

    if((0 >= fd) || (NULL == group)) return;
    if(NULL == (g_info = getgrnam(group))) return;
    memset(&file_info, 0x00, sizeof(struct stat));
    if(0 > fstat(fd, &file_info)) return;
    fchown(fd, file_info.st_uid, g_info->gr_gid);
}
VOID swc_fchgrp(IN INT fd, IN WCString group)
{
    swc_fchgrp(fd, LPCSTR(group));
}
SWC_BOOL swc_parse_file_name_from_path(IN const CHAR* spath, OUT CHAR* sout, IN INT nlen)
{
	if((NULL == spath) || (NULL == sout) || (0 == nlen)) return FALSE;
	if(0 == strlen(spath)) return FALSE;

	std::string sbuf    = spath, sfname;
	size_t      nfind   = sbuf.find_last_of('/');
	INT         nlength = sbuf.length();

	if(sbuf.npos == nfind) return FALSE;
	if((size_t)nlength >= (nfind + 1))
	{
		sfname = sbuf.substr(nfind + 1);
		snprintf(sout, nlen, "%s", sfname.data());
	}
	else return FALSE;
	return TRUE;
}
SWC_BOOL swc_chk_proc_exist(IN const CHAR* proc)
{
	DIR*           pdir   = NULL;
	struct dirent* pentry = NULL;
	CHAR           sorgpath[SWC_MAX_PATH_LEN] = {0};
	CHAR           sfilepath[512] = {0}, sexepath[512] = {0}, sexename[512] = {0};
	SWC_BOOL           bret   = FALSE;
	INT            mypid  = (int)getpid();

	if(NULL == proc) return FALSE;

	getcwd(sorgpath, SWC_MAX_PATH_LEN);
	chdir("/proc");
	if(NULL == (pdir = opendir("/proc")))
	{
		return FALSE;
	}
	while(NULL != (pentry = readdir(pdir)))
	{
		if((0 != atoi(pentry->d_name)) || (DT_DIR != pentry->d_type))
		{
			INT ncurpid = 0;

			memset(sexepath,  0x00, 512);
			memset(sfilepath, 0x00, 512);
			memset(sexename,  0x00, 512);
			ncurpid = (int)atoi(pentry->d_name);
			if((0 < ncurpid) && (mypid != ncurpid) && (0 != strcmp("self", pentry->d_name)))
			{
				snprintf(sfilepath, 512, "/proc/%s/exe", pentry->d_name);
				if(0 < readlink(sfilepath, sexepath, 512))
				{
					swc_parse_file_name_from_path(sexepath, sexename, 512);
					//if(0 == strcmp(proc, sexename))
					if (NULL != strstr(sexename, proc))
					{
						bret = TRUE;
						break;
					}
				}
			}
		}
	}
	closedir(pdir);
	chdir(sorgpath);
	return bret;
}
SWC_BOOL swc_chk_proc_exist(IN WCString proc)
{
    return swc_chk_proc_exist(LPCSTR(proc));
}
VOID swc_send_signal_to_proc(IN const CHAR* proc, IN INT signo)
{
	DIR*           pdir   = NULL;
	struct dirent* pentry = NULL;
	CHAR           sorgpath[SWC_MAX_PATH_LEN] = {0};
	CHAR           sfilepath[512] = {0}, sexepath[512] = {0}, sexename[512] = {0};
	INT            mypid  = (int)getpid();

	if(NULL == proc) return;

	getcwd(sorgpath, SWC_MAX_PATH_LEN);
	chdir("/proc");
	if(NULL == (pdir = opendir("/proc")))
	{
		return;
	}
	while(NULL != (pentry = readdir(pdir)))
	{
		if((0 != atoi(pentry->d_name)) || (DT_DIR != pentry->d_type))
		{
			INT ncurpid = 0;

			memset(sexepath,  0x00, 512);
			memset(sfilepath, 0x00, 512);
			memset(sexename,  0x00, 512);
			ncurpid = (int)atoi(pentry->d_name);
			if((mypid != ncurpid) && (0 != strcmp("self", pentry->d_name)))
			{
				snprintf(sfilepath, 512, "/proc/%s/exe", pentry->d_name);
				if(0 < readlink(sfilepath, sexepath, 512))
				{
					swc_parse_file_name_from_path(sexepath, sexename, 512);
					if(0 == strcmp(proc, sexename))
					{
						kill(ncurpid, signo);
					}
				}
			}
		}
	}
	closedir(pdir);
	chdir(sorgpath);
	return;
}
VOID swc_send_signal_to_proc(IN WCString proc, IN INT signo)
{
    swc_send_signal_to_proc(LPCSTR(proc), signo);
}
CHAR* swc_cls_apostrophe(IN CHAR* strbuf)
{
    INT str_len = 0, i = 0;

    if(NULL == strbuf) return NULL;
    if(0 == (str_len = strlen(strbuf))) return strbuf;
    for(i = 0; i < str_len; i++)
        if(39 == strbuf[i]) strbuf[i] = 32;
    return strbuf;
}
CHAR* swc_cls_comma(IN CHAR* strbuf)
{
    INT str_len = 0, i = 0;

    if(NULL == strbuf) return NULL;
    if(0 == (str_len = strlen(strbuf))) return strbuf;
    for(i = 0; i < str_len; i++)
        if(',' == strbuf[i]) strbuf[i] = 46;
    return strbuf;
}
CHAR* swc_cls_linefeed(IN CHAR* strbuf)
{
    INT str_len = 0, i = 0;

    if(NULL == strbuf) return NULL;
    if(0 == (str_len = strlen(strbuf))) return strbuf;
    for(i = 0; i < str_len; i++)
        if(('\n' == strbuf[i]) || ('\r' == strbuf[i])) strbuf[i] = 32;
    return strbuf;
}
CHAR* swc_cvt_chr_chr(IN CHAR* strbuf, IN CHAR cfrom, IN CHAR cto)
{
    INT str_len = 0, i = 0;

    if(NULL == strbuf) return NULL;
    if(0 == (str_len = strlen(strbuf))) return strbuf;
    for(i = 0; i < str_len; i++)
        if(cfrom == strbuf[i]) strbuf[i] = cto;
    return strbuf;
}
CHAR* swc_str_chr_right(IN const CHAR* src, IN const CHAR find, OUT CHAR** ptrptr)
{
    CHAR* pfind = NULL;

    if(NULL != src) pfind = (PCHAR)strchr(src, find);
    else if(NULL == *ptrptr) return NULL;
    else pfind = strchr(*ptrptr, find);
    if(NULL != pfind) *ptrptr = pfind + 1;
    else
    {
        *ptrptr = NULL;
        return NULL;
    }
    return pfind;
}
CHAR* swc_cvt_chr_str(IN const CHAR* src,
                      OUT CHAR       *dst,
                      IN INT         ndstlen,
                      IN const CHAR  cfind,
                      IN const CHAR* scvt)
{
    CHAR *popt = NULL, *pret = NULL, *ppivot = NULL;

    if((NULL == src) || (NULL == dst) || (0 == ndstlen)) return NULL;
    if(0 == strlen(src)) return NULL;
    if(NULL != (pret = swc_str_chr_right(src, cfind, &popt)))
    {
        strncpy(dst, src, strlen(src) - strlen(pret));
        strcat(dst, scvt);
        ppivot = popt;
    }
    else
    {
        snprintf(dst, ndstlen, "%s", src);
        return dst;
    }
    while(1)
    {
        if(NULL != (pret = swc_str_chr_right(NULL, cfind, &popt)))
        {
            strncat(dst, ppivot, strlen(ppivot) - strlen(pret));
            strcat(dst, scvt);
            ppivot = popt;
        }
        else
        {
            if(0 < strlen(ppivot)) strcat(dst, ppivot);
            break;
        }
    }
    return dst;
}
CHAR* swc_str_chr_right_csv(IN const CHAR* src, IN const CHAR find, OUT CHAR** ptrptr)
{
    CHAR* pfind = NULL, *pdq = NULL;

    if(NULL != src)
    {
        pfind = (PCHAR)strchr(src, find);
        pdq = (PCHAR)strchr(src, '"');
    }
    else if(NULL == *ptrptr) return NULL;
    else
    {
        pfind = strchr(*ptrptr, find);
        pdq = strchr(*ptrptr, '"');
    }
    if(NULL != pdq)
    {
        if(pdq < pfind)
        {
            pfind = strchr(pdq + 1, '"');
            if(NULL != pfind)
            {
                pfind = strchr(pfind + 1, find);
                if(NULL != pfind) *ptrptr = pfind + 1;
                else { *ptrptr = NULL; return NULL; }
            }
        }
        else
        {
            if(NULL != pfind) *ptrptr = pfind + 1;
            else { *ptrptr = NULL; return NULL; }
        }
    }
    else
    {
        if(NULL != pfind) *ptrptr = pfind + 1;
        else { *ptrptr = NULL; return NULL; }
    }
    return pfind;
}
INT swc_iconv(IN CHAR*       inbuf,
              OUT CHAR*      outbuf,
              IN INT         outlen,
              IN const CHAR* setfrom,
              IN const CHAR* setto)
{
    iconv_t cd = 0;
    INT nret = 0;
    size_t inl = 0, outl = outlen;
    CHAR *in = inbuf;
    CHAR *out = outbuf;
    CHAR fbuf[40] = {0}, tbuf[40] = {0};

    if((NULL == inbuf) || (NULL == outbuf) || (NULL == setfrom) || (NULL == setto))
        return -1;
    if( (0 == outlen) || (0 == (inl = strlen(inbuf))) ||
        (0 == strlen(setfrom)) || (0 == strlen(setto))) return -1;
    snprintf(fbuf, 40, "%s//IGNORE", setfrom); snprintf(tbuf, 40, "%s", setto);
    if(NULL != strstr(fbuf, "NONE")) snprintf(fbuf, 40, "MS-ANSI");
    if(NULL != strstr(tbuf, "NONE")) snprintf(tbuf, 40, "MS-ANSI");
    if(0 == strcmp(tbuf, fbuf))
    {
        snprintf(outbuf, outlen, "%s", inbuf);
        nret = 0;
    }
    else
    {
        memset(outbuf, 0x00, outlen);
        if((iconv_t)0 > (cd = iconv_open(tbuf, fbuf)))
        {
            return -2;
        }
        if(0 > (nret = iconv(cd, &in, &inl, &out, &outl)))
        {
            iconv_close(cd);
            return -3;
        }
        iconv_close(cd);
    }
    return nret;
}
CHAR* swc_toupper(IN CHAR* strbuf)
{
    INT str_len = 0, i = 0;

    if(NULL == strbuf) return NULL;
    if(0 == (str_len = strlen(strbuf))) return strbuf;
    for(i = 0; i < str_len; i++)
    strbuf[i] = toupper(strbuf[i]);
    return strbuf;
}
SWC_BOOL swc_is_file_exists(IN const CHAR* sdir, IN const CHAR* sfile)
{
    SWC_BOOL       bret   = FALSE;
    DIR*           pdir   = NULL;
    struct dirent* pentry = NULL;
    struct stat    statbuf;
    CHAR           sorgpath[SWC_MAX_PATH_LEN] = {0};

    if(NULL == sdir) return FALSE;
    if(0 == strlen(sdir)) return FALSE;
    if(NULL == sfile) return FALSE;
    if(0 == strlen(sfile)) return FALSE;

    getcwd(sorgpath, SWC_MAX_PATH_LEN);
    if(NULL == (pdir = opendir(sdir))) return FALSE;
    chdir(sdir);
    while(NULL != (pentry = readdir(pdir)))
    {
        stat(pentry->d_name, &statbuf);
        if(0 == S_ISDIR(statbuf.st_mode))
        {
            if(0 == strcmp(sfile, pentry->d_name))
            {
                bret = TRUE;
                break;
            }
        }
    }
    closedir(pdir);
    chdir(sorgpath);
    return bret;
}
SWC_BOOL swc_is_file_exists(IN WCString sdir, IN WCString sfile)
{
    return swc_is_file_exists(LPCSTR(sdir), LPCSTR(sfile));
}
UINT swc_cnt_match_file_in_dir(IN const CHAR* sdir, IN const CHAR* slike)
{
    UINT           nret   = 0;
    DIR*           pdir   = NULL;
    struct dirent* pentry = NULL;
    struct stat    statbuf;
    CHAR           sorgpath[SWC_MAX_PATH_LEN] = {0};

    if(NULL == sdir) return 0;
    if(0 == strlen(sdir)) return 0;
    if(NULL == slike) return 0;
    if(0 == strlen(slike)) return 0;

    getcwd(sorgpath, SWC_MAX_PATH_LEN);
    if(NULL == (pdir = opendir(sdir))) return 0;
    chdir(sdir);
    while(NULL != (pentry = readdir(pdir)))
    {
        stat(pentry->d_name, &statbuf);
        if(0 == S_ISDIR(statbuf.st_mode)) // file
        {
            if(NULL != strstr(pentry->d_name, slike))
            {
				nret++;
            }
        }
		else // directory
		{
			if((0 == strcmp(pentry->d_name, ".")) || (0 == strcmp(pentry->d_name, "..")))
			{
				continue;
			}
			else
			{
				std::string scurpath;

				scurpath = sdir;
				scurpath += '/';
				scurpath += pentry->d_name;
				nret += swc_cnt_match_file_in_dir(scurpath.data(), slike);
			}
		}
    }
    closedir(pdir);
    chdir(sorgpath);
    return nret;
}
UINT swc_cnt_match_file_in_dir(IN WCString sdir, IN WCString slike)
{
	return swc_cnt_match_file_in_dir(LPCSTR(sdir), LPCSTR(slike));
}
UINT swc_get_matched_extension_files_in_dir(IN const CHAR* sDir, IN const CHAR* sExt, OUT WCArray<WCString>& arrPath)
{
	UINT           nRet = 0;
	DIR*           pDir = NULL;
	struct dirent* pEntry = NULL;
	struct stat    statBuf;
	CHAR           sOrgPath[SWC_MAX_PATH_LEN] = {0};

	if (NULL == sDir) return 0;
	if (0 == strlen(sDir)) return 0;
	
	getcwd(sOrgPath, SWC_MAX_PATH_LEN);
	if (NULL == (pDir = opendir(sDir)))
	{
		fprintf(stderr, "fail open directory: '%s', errno:%d\n", sDir, errno);
		perror("opendir()");
		return 0;
	}
	chdir(sDir);
	while (NULL != (pEntry = readdir(pDir)))
	{
		stat(pEntry->d_name, &statBuf);
		if (0 ==S_ISDIR(statBuf.st_mode))
		{
			WCString sUpExt;
			WCString sUpFilePath = pEntry->d_name;

			sUpExt.Format(".%s", sExt);
			sUpExt.MakeUpper();
			sUpFilePath.MakeUpper();
			if (0 <= sUpFilePath.ReverseFind(sUpExt))
			{
                WCString sFilePath = "";
				sFilePath.Format("%s/%s", sDir, pEntry->d_name);
				arrPath.Add(sFilePath);
				nRet++;
			}
		}
	}
    closedir(pDir);
    chdir(sOrgPath);
	return nRet;
}
SWC_BOOL swc_is_dir_exists(IN const CHAR* sdir)
{
	DIR* pdir = NULL;

	if(NULL == (pdir = opendir(sdir))) return FALSE;
	closedir(pdir);
	return TRUE;
}
SWC_BOOL swc_is_dir_exists(IN WCString sdir)
{
	return swc_is_dir_exists(LPCSTR(sdir));
}
SWC_BOOL swc_rmdir(IN const CHAR* sdir)
{
	SWC_BOOL       bret   = TRUE;
	DIR*           pdir   = NULL;
	struct dirent* pentry = NULL;
	struct stat    statbuf;
	CHAR           sorgpath[SWC_MAX_PATH_LEN] = {0};

	if(NULL == sdir) return FALSE;
	if(0 == strlen(sdir)) return FALSE;

	getcwd(sorgpath, SWC_MAX_PATH_LEN);
	if(NULL == (pdir = opendir(sdir))) return FALSE;
	chdir(sdir);
	while(NULL != (pentry = readdir(pdir)))
	{
		stat(pentry->d_name, &statbuf);
		if(0 == S_ISDIR(statbuf.st_mode)) // file
		{
			std::string scurfile;

			scurfile = sdir;
			scurfile += '/';
			scurfile += pentry->d_name;

			unlink(scurfile.data());
		}
		else // directory
		{
			if((0 == strcmp(pentry->d_name, ".")) || (0 == strcmp(pentry->d_name, "..")))
			{
				continue;
			}
			else
			{
				std::string scurpath;

				scurpath = sdir;
				scurpath += '/';
				scurpath += pentry->d_name;

				bret = swc_rmdir(scurpath.data());
				rmdir(scurpath.data());
			}
		}
	}
	closedir(pdir);
	rmdir(sdir);
	chdir(sorgpath);
	return bret;
}
UINT swc_get_file_size(IN const CHAR* spath)
{
	UINT lret = 0;
	struct stat st;

	if(NULL == spath) return 0;
	if(0 == strlen(spath)) return 0;
	memset(&st, 0x00, sizeof(struct stat));
	if(0 > lstat(spath, &st)) return 0;
	lret = (UINT)st.st_size;

	return lret;
}
UINT swc_get_file_size(IN WCString spath)
{
	return swc_get_file_size(LPCSTR(spath));
}
SWC_BOOL swc_is_file_size_over(IN const CHAR* spath, IN UINT lsize)
{
    SWC_BOOL bret = FALSE;
    struct stat st;

    if(NULL == spath) return bret;
    if(0 == strlen(spath)) return bret;
    memset(&st, 0x00, sizeof(struct stat));
    if(0 > lstat(spath, &st)) return bret;
    if(lsize < (UINT)st.st_size) bret = TRUE;
    return bret;
}
SWC_BOOL swc_is_file_size_over(IN WCString spath, IN UINT lsize)
{
    return swc_is_file_size_over(LPCSTR(spath), lsize);
}
SWC_BOOL swc_is_unicode_file(IN const CHAR* spath)
{
    SWC_BOOL bret = FALSE;
    INT fd = -1;
    UCHAR sbuf[10] = {0};

    if(-1 == (fd = open(spath, O_RDONLY))) return bret;
    read(fd, sbuf, 10);
    if((sbuf[0] == 0xFF) && (sbuf[1] == 0xFF)) bret = TRUE;
    close(fd);
    return bret;
}
VOID swc_sleep(IN INT _sec, IN INT _usec)
{
    struct timespec req;

    if((0 == _sec) && (0 == _usec))
    {
        req.tv_sec = 0;
        req.tv_nsec = 999999999L;
    }
    else
    {
//        if(10000 > _usec) _usec = 10000;
        req.tv_sec = _sec;
        req.tv_nsec = _usec * 1000;
    }
    nanosleep(&req, &req);
/*
    UINT ltime = 0;

    if(0 < _sec)  ltime = _sec * 1000000;
    if(0 < _usec) ltime += _usec;
    usleep(ltime);


    struct timeval _tv;
    _tv.tv_sec = _sec; _tv.tv_usec = _usec;
    select(1, NULL, NULL, NULL, &_tv);
*/
}
SWC_BOOL swc_get_remote_status(IN const CHAR* seth, IN const CHAR* srcip, IN const CHAR* dstip)
{
    SWC_BOOL bret = FALSE;
    FILE* fp = NULL;
    CHAR scmd[1024] = {0};
    CHAR linebuf[1024] = {0};

    if((NULL == seth) || (NULL == srcip) || (NULL == dstip)) return bret;
    if((NULL == seth) || (0 == strlen(srcip)) || (0 == strlen(dstip))) return bret;
    snprintf(scmd, 1024, "arping -c 5 -I %s -s %s %s | grep Received ",
                seth, srcip, dstip);
    if(NULL == (fp = popen(scmd, "r"))) return bret;
    while(fgets(linebuf, sizeof(linebuf), fp))
    {
        if(0 < strlen(linebuf))
        {
            CHAR wbuf[3][128] = {{0}};

            sscanf(linebuf, "%s %s %s", wbuf[0], wbuf[1], wbuf[2]);
            if(NULL != wbuf[1])
            {
                if(0 < strlen(wbuf[1]))
                {
                    UINT lcnt = (UINT)strtoul(wbuf[1], NULL, 10);

                    if(0 < lcnt) bret = TRUE;
                    break;
                }
            }
        }
    }
    if(NULL != fp) pclose(fp);
    return bret;
}
SWC_BOOL swc_get_remote_status(IN WCString seth, IN WCString srcip, IN WCString dstip)
{
    return swc_get_remote_status(LPCSTR(seth), LPCSTR(srcip), LPCSTR(dstip));
}
SWC_BOOL swc_get_remote_status(IN const CHAR* seth, IN UINT srcip, IN UINT dstip)
{
    CHAR sbuf1[16] = {0}, sbuf2[16] = {0};
    struct in_addr ip_1, ip_2;

    if((NULL == seth) || (0 == srcip) || (0 == dstip)) return FALSE;

    ip_1.s_addr = srcip;
    ip_2.s_addr = dstip;
    snprintf(sbuf1, 16, "%s", inet_ntoa(ip_1));
    snprintf(sbuf2, 16, "%s", inet_ntoa(ip_2));
    return swc_get_remote_status(seth, sbuf1, sbuf2);
}
SWC_BOOL swc_get_remote_status(IN WCString seth, IN UINT srcip, IN UINT dstip)
{
    return swc_get_remote_status(LPCSTR(seth), srcip, dstip);
}
SWC_BOOL swc_is_eth_exists(IN const CHAR* seth)
{
#ifdef __SWC_OS_LINUX__	
    SWC_BOOL bret = FALSE;
    INT sfd = socket(PF_INET, SOCK_STREAM, 0), i = 0;

    for(i = 1;; i++)
    {
        struct ifreq ifr;

        ifr.ifr_ifindex = i;
        if(0 > ioctl(sfd, SIOCGIFNAME, &ifr)) break;
        if(0 > ioctl(sfd, SIOCGIFADDR, &ifr)) continue;
        if(NULL != strstr(ifr.ifr_ifrn.ifrn_name, seth))
        {
            bret = TRUE;
            break;
        }
    }
    close(sfd);
    return bret;
#else
	return FALSE;
#endif
}
SWC_BOOL swc_is_eth_exists(IN WCString seth)
{
    return swc_is_eth_exists(LPCSTR(seth));
}
UINT swc_get_eth_ip(IN const CHAR* seth, IN CHAR* sout, IN INT nlen)
{
#ifdef __SWC_OS_LINUX__
    UINT lret = 0;
    INT sfd = socket(PF_INET, SOCK_STREAM, 0), i = 0;

    for(i = 1;; i++)
    {
        struct ifreq ifr;
        struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;

        ifr.ifr_ifindex = i;
        if(0 > ioctl(sfd, SIOCGIFNAME, &ifr)) break;
        if(0 > ioctl(sfd, SIOCGIFADDR, &ifr)) continue;
        if(NULL != strstr(ifr.ifr_ifrn.ifrn_name, seth))
        {
            memcpy(&lret, &sin->sin_addr, 4);
            if(NULL != sout) snprintf(sout, nlen, "%s", inet_ntoa(sin->sin_addr));
            break;
        }
    }
    close(sfd);
    return lret;
#else
	return 0;
#endif
}
UINT swc_get_eth_ip(IN WCString seth, IN WCString& sout, IN INT nlen)
{
#ifdef __SWC_OS_LINUX__
    UINT lret = 0;
    INT sfd = socket(PF_INET, SOCK_STREAM, 0), i = 0;

    for(i = 1;; i++)
    {
        struct ifreq ifr;
        struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;

        ifr.ifr_ifindex = i;
        if(0 > ioctl(sfd, SIOCGIFNAME, &ifr)) break;
        if(0 > ioctl(sfd, SIOCGIFADDR, &ifr)) continue;
        if(NULL != strstr(ifr.ifr_ifrn.ifrn_name, LPCSTR(seth)))
        {
            memcpy(&lret, &sin->sin_addr, 4);
            sout = inet_ntoa(sin->sin_addr);
            break;
        }
    }
    close(sfd);
    return lret;
#else
	return 0;
#endif
}
UINT swc_get_1st_mac(OUT UCHAR cmac[])
{
#ifdef __SWC_OS_LINUX__
	UINT lret = 0;
	INT sfd = socket(PF_INET, SOCK_STREAM, 0), i = 0, j = 0;
	if(0 > sfd) return 0;

	for(i = 1;; i++)
	{
		struct ifreq ifr;
		struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;
		SWC_BOOL   bzeromac = TRUE;

		ifr.ifr_ifindex = i;

		if(0 > ioctl(sfd, SIOCGIFNAME, &ifr)) break;

		// check flag
		if(0 > ioctl(sfd, SIOCGIFFLAGS, &ifr)) continue;

//		if(!(ifr.ifr_flags & IFF_RUNNING)) continue;
		if(ifr.ifr_flags & IFF_DEBUG) continue;
		if(ifr.ifr_flags & IFF_LOOPBACK) continue;
		if(ifr.ifr_flags & IFF_NOARP) continue;

		// extract ip 
		if(0 > ioctl(sfd, SIOCGIFADDR, &ifr)) continue;
		memcpy(&lret, &sin->sin_addr, 4);

		// extract mac address
		if(0 > ioctl(sfd, SIOCGIFHWADDR, &ifr)) continue;
		for(j = 0; j < 6; j++) { if(0 != (ifr.ifr_hwaddr.sa_data[j] & 0xff)) { bzeromac = FALSE; break; } }
		if(TRUE == bzeromac) continue;

		for(j = 0; j < 6; j++) cmac[j] = (ifr.ifr_hwaddr.sa_data[j] & 0xff);

		break;
	}
	close(sfd);
	return lret;
#else
	return 0;
#endif
}
SWC_BOOL swc_get_dns(OUT CHAR* sdns, IN INT nlen)
{
    CHAR sbuf[100] = {0}, linebuf[SWC_MAX_LINE_BUF_LEN] = {0};
    CHAR wbuf[2][1024] = {{0}};
    FILE* pfile = NULL;

    if(NULL == sdns) return FALSE;
    snprintf(sbuf, 100, "%s", sdns);
    if(NULL == (pfile = fopen("/etc/resolv.conf", "r"))) return FALSE;
    while(fgets(linebuf, sizeof(linebuf), pfile))
    {
        sscanf(linebuf, "%s %s", wbuf[0], wbuf[1]);
        if(NULL != wbuf[1])
        {
            snprintf(sdns, nlen, "%s", wbuf[1]);
            break;
        }
    }
    fclose(pfile);
    return TRUE;
}
WCString swc_get_dns()
{
    CHAR linebuf[SWC_MAX_LINE_BUF_LEN] = {0};
    CHAR wbuf[2][1024] = {{0}};
    FILE* pfile = NULL;
    WCString sret("");

    if(NULL == (pfile = fopen("/etc/resolv.conf", "r"))) return sret;
    while(fgets(linebuf, sizeof(linebuf), pfile))
    {
        sscanf(linebuf, "%s %s", wbuf[0], wbuf[1]);
        if(NULL != wbuf[1])
        {
            sret = wbuf[1];
            sret.Trim();
            break;
        }
    }
    fclose(pfile);
    return sret;
}
VOID swc_set_dns(IN const CHAR* sdns)
{
    FILE* pfile = NULL;

    if(NULL == sdns) return;
    if(0 == strlen(sdns)) return;
    if(NULL == (pfile = fopen("/etc/resolv.conf", "w+"))) return;
    fprintf(pfile, "nameserver %s", sdns);
    fflush(pfile);
    fclose(pfile);
}
VOID swc_set_dns(IN WCString sdns)
{
    swc_set_dns(LPCSTR(sdns));
}
VOID swc_get_local_time(IN time_t ltime, OUT WCString& sret)
{
	CHAR stime[30] = {0};
	struct tm tm_ptr;

	if(0 < ltime)
	{
		memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
		strftime(stime, 30, "%Y-%m-%d %H:%M:%S", &tm_ptr);
	}
	else snprintf(stime, 30, "0000-00-00 00:00:00");
	sret = stime;
}
VOID swc_get_global_time(IN time_t ltime, OUT WCString& sret)
{
	CHAR stime[30] = {0};
	struct tm tm_ptr;

	if(0 < ltime)
	{
		memcpy(&tm_ptr, gmtime(&ltime), sizeof(struct tm));
		strftime(stime, 30, "%Y-%m-%d %H:%M:%S", &tm_ptr);
	}
	else snprintf(stime, 30, "0000-00-00 00:00:00");
	sret = stime;
}
time_t swc_lt_to_gt(IN IN time_t lt)
{
	time_t tcurtime = 0, tlot = 0, tgmt = 0, tret = 0;
	time_t lgap = 0;
	struct tm tm_lt, tm_gt;

	if(0 == lt) return 0;

	time(&tcurtime);
	memcpy(&tm_lt, localtime(&tcurtime), sizeof(struct tm));
	memcpy(&tm_gt, gmtime(&tcurtime), sizeof(struct tm));
	tlot = mktime(&tm_lt);
	tgmt = mktime(&tm_gt);

	lgap = (time_t)(tgmt - tlot);
	tret = (time_t)(lt + lgap);

	return tret;
}
time_t swc_gt_to_lt(IN time_t gt)
{
	time_t tcurtime = 0, tlot = 0, tgmt = 0, tret = 0;
	time_t lgap = 0;
	struct tm tm_lt, tm_gt;

	if(0 == gt) return 0;

	time(&tcurtime);
	memcpy(&tm_lt, localtime(&tcurtime), sizeof(struct tm));
	memcpy(&tm_gt, gmtime(&tcurtime), sizeof(struct tm));
	tlot = mktime(&tm_lt);
	tgmt = mktime(&tm_gt);

	lgap = (time_t)(tlot - tgmt);
	tret = (time_t)(gt + lgap);

	return tret;
}
WCString ul2s(IN UINT ulval)
{
    WCString sret;
    sret.Format("%u", ulval);
    return sret;
}
WCString l2s(IN INT lval)
{
    WCString sret;
    sret.Format("%d", lval);
    return sret;
}
WCString i2s(IN INT ival)
{
    WCString sret;
    sret.Format("%d", ival);
    return sret;
}
WCString sh2s(IN SHORT shval)
{
    WCString sret;
    sret.Format("%d", shval);
    return sret;
}
/* ----------------------------------------------------------------------------
	description : convert time from integer to string
				  format : 2007/02/09 12:12:12
	parameters  :
		- (UINT) time integer
		- (CHAR*)  out buffer
		- (int)    buffer length
	return      : ()
	history     :
		- 2007. 02. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID time_ltos(IN UINT ltime, OUT CHAR* stime, IN INT len)
{
	struct tm time_ptr;
	time_t cpytime = (time_t)ltime;

	if((30 > len) || (NULL == stime)) return;
	if(0 == ltime)
	{
		snprintf(stime, len, "0000/00/00 00:00:00");
	}
	else
	{
		memcpy(&time_ptr, localtime(&cpytime), sizeof(struct tm));
		strftime(stime, 30, "%Y/%m/%d %H:%M:%S", &time_ptr);
	}
}
/* ----------------------------------------------------------------------------
	description : make time given each elements of date
	parameters  :
		- (int) year 
		- (int) month  (1 ~ 12)
		- (int) day    (1 ~ 31)
		- (int) hour   (1 ~ 24)
		- (int) minute (1 ~ 59)
		- (int) second (1 ~ 59)
	return      : (time_t) return time 
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
time_t time_vtol(IN INT year, IN INT mon, IN INT day, IN INT hour, IN INT min, IN INT sec)
{
	struct tm tm_ptr;

	tm_ptr.tm_year  = year - 1900;
	tm_ptr.tm_mon   = mon - 1;
	tm_ptr.tm_mday  = day;
	tm_ptr.tm_hour  = hour;
	tm_ptr.tm_min   = min;
	tm_ptr.tm_sec   = sec;
	tm_ptr.tm_isdst = -1;
	return mktime(&tm_ptr);
}

/* ----------------------------------------------------------------------------
	description : make time given string time
	parameters  :
		(const CHAR*) time string
	return      : (time_t) return time 
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
time_t time_stol(IN const CHAR* strdate)
{
	CHAR syear[5] = {0}, smon[3] = {0}, sday[3] = {0};
	CHAR shour[3] = {0}, smin[3] = {0}, ssec[3] = {0};
	INT  iyear = 0, imon = 0, iday = 0, ihour = 0, imin = 0, isec = 0;

	if(NULL == strdate) return 0;
	if(18 > strlen(strdate)) return 0;

	syear[0] = strdate[0]; syear[1] = strdate[1]; syear[2] = strdate[2];
	syear[3] = strdate[3]; syear[4] = '\0';
	smon[0] = strdate[5]; smon[1] = strdate[6]; smon[2] = '\0';
	sday[0] = strdate[8]; sday[1] = strdate[9]; sday[2] = '\0';
	shour[0] = strdate[11]; shour[1] = strdate[12]; shour[2] = '\0';
	smin[0] = strdate[14]; smin[1] = strdate[15]; smin[2] = '\0';
	ssec[0] = strdate[17]; ssec[1] = strdate[18]; ssec[2] = '\0';

	iyear = atoi(syear); imon = atoi(smon); iday = atoi(sday);
	ihour = atoi(shour); imin = atoi(smin); isec = atoi(ssec);

	if((2038 <= iyear) || (0 == iyear)) return 0;
	return time_vtol(iyear, imon, iday, ihour, imin, isec);
}

/* ----------------------------------------------------------------------------
	description : extract year from time 
	parameters  :
		(time_t) time value 
	return      : (int) return year
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_year(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return (tm_ptr.tm_year + 1900);
}

/* ----------------------------------------------------------------------------
	description : extract month from time value
	parameters  :
		(time_t) time value 
	return      : (int) return month
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_mon(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return (tm_ptr.tm_mon + 1);
}

/* ----------------------------------------------------------------------------
	description : extract day from time value
	parameters  :
		(time_t) time value
	return      : (int) return day
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_day(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return tm_ptr.tm_mday;
}

/* ----------------------------------------------------------------------------
	description : extract hour from time value 
	parameters  :
		(time_t) time value
	return      : (int) return hour
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_hour(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return tm_ptr.tm_hour;
}

/* ----------------------------------------------------------------------------
	description : extract minute from time value
	parameters  :
		(time_t) time value 
	return      : (int) return minute
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_min(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return tm_ptr.tm_min;
}

/* ----------------------------------------------------------------------------
	description : extract second from time value
	parameters  :
		(time_t) time value 
	return      : (int) return second
	history     :
		- 2007. 10. 30 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_sec(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return tm_ptr.tm_sec;
}

/* ----------------------------------------------------------------------------
	description : extrct day of week from time value
	parameters  :
		(time_t) time value 
	return      : (int) return second
	history     :
		- 2014. 02. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_day_of_week(IN time_t ltime)
{
	struct tm tm_ptr;

	memcpy(&tm_ptr, localtime(&ltime), sizeof(struct tm));
	return tm_ptr.tm_wday;
}

/* ----------------------------------------------------------------------------
	description : extrct week of month from time value
	parameters  :
		(time_t) time value 
	return      : (int) return second
	history     :
		- 2014. 02. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT time_get_week_of_mon(IN time_t ltime)
{
    struct tm InputTm, FstDayTm;
    time_t tFst = 0;
    INT    nYear = 0, nMon = 0, nDay = 0, nDow = 0, nFstDayDow = 0, nCnt = 0;

    memcpy(&InputTm, localtime(&ltime), sizeof(struct tm));
    nYear      = InputTm.tm_year + 1900;
    nMon       = InputTm.tm_mon + 1;
    nDay       = InputTm.tm_mday;
    nDow       = InputTm.tm_wday;
    tFst       = SWC::time_vtol(nYear, nMon, 1, 0, 0, 0);
    memcpy(&FstDayTm, localtime(&tFst), sizeof(struct tm));
    nFstDayDow = FstDayTm.tm_wday;
    do { nDay -= 7; nCnt++; } while(nDay > 0);
    if(nDow < nFstDayDow) nCnt++;
	return nCnt;
}

/* ----------------------------------------------------------------------------
	description : make mac given upper 4 bytes and lower 2bytes values
	parameters  :
		(CHAR*)  out mac string
		(int)    buffer length
		(UINT) in upper bytes
		(UINT) in lower bytes
	return      : (int) return second
	history     :
		- 2008. 08. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID swc_get_smac_s(OUT CHAR* smac, IN INT nlen, IN UINT lmacu, IN UINT lmacl)
{
    UCHAR cmacu[4] = {0}, cmacl[4] = {0}, cmac[6] = {0};
    INT i = 0;

    memcpy(cmacu, &lmacu, 4);
    memcpy(cmacl, &lmacl, 4);

    for(i = 0; i < 4; i++) cmac[i]     = cmacu[i];
    for(i = 0; i < 2; i++) cmac[i + 4] = cmacl[i];

    snprintf(smac, nlen, "%02X%02X%02X%02X%02X%02X",
            cmac[0], cmac[1], cmac[2], cmac[3], cmac[4], cmac[5]);
}

/* ----------------------------------------------------------------------------
	description : make mac given upper 4 bytes and lower 2bytes values
	parameters  :
		(WCString&) out mac string
		(UINT)    in upper bytes
		(UINT)    in lower bytes
	return      : (int) return second
	history     :
		- 2008. 08. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID swc_get_smac_w(OUT WCString& smac, IN UINT lmacu, IN UINT lmacl)
{
    UCHAR cmacu[4] = {0}, cmacl[4] = {0}, cmac[6] = {0};
    INT i = 0;

    memcpy(cmacu, &lmacu, 4);
    memcpy(cmacl, &lmacl, 4);

    for(i = 0; i < 4; i++) cmac[i]     = cmacu[i];
    for(i = 0; i < 2; i++) cmac[i + 4] = cmacl[i];

    smac.Format("%02X%02X%02X%02X%02X%02X",
            cmac[0], cmac[1], cmac[2], cmac[3], cmac[4], cmac[5]);
}

/* ----------------------------------------------------------------------------
	description : get peer ip address integer
	parameters  :
			(int) socket descriptor
	return      : (UINT) return ip addres integer 
	history     :
		- 2008. 08. 14 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT swc_get_peer_ip(IN INT sfd)
{
	sockaddr_in peeraddr;
	socklen_t   llen = sizeof(struct sockaddr_in);
	UINT lret = 0;

	if(0 >= sfd) return 0;
	if(0 > getpeername(sfd, (struct sockaddr*)&peeraddr, &llen))
	{
		return 0;
	}
	lret = (UINT)htonl(peeraddr.sin_addr.s_addr);
	return lret;
}

/* ----------------------------------------------------------------------------
	description : get CPU count
	parameters  :
			()
	return      : (UINT) return CPU count
	history     :
		- 2008. 10. 16 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT swc_get_cpu_count()
{
	FILE* fp = NULL;
	CHAR linebuf[100] = {0};
	UINT lret = 1;

	if(NULL == (fp = popen("getconf  _NPROCESSORS_CONF", "r"))) return 1;
	if(fgets(linebuf, sizeof(linebuf), fp))
	{
		if(0 < strlen(linebuf))
		{	
			lret = (UINT)strtoul(linebuf, NULL, 10);
		}
	}
	if(NULL != fp) pclose(fp);
	return lret;
}

/* ----------------------------------------------------------------------------
	description : convert from string mac to raw mac
	parameters  :
			(const CHAR*) input string MAC
			(UCHAR*) output raw MAC
	return      : ()
	history     :
		- 2011. 08. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID swc_smac_to_cmac(IN const CHAR* smac, OUT UCHAR cmac[])
{
	UCHAR mac_buf[6][3];
	INT i = 0;

	for(i = 0; i < 6; i++)
	{
		mac_buf[i][0] = smac[i*2];
		mac_buf[i][1] = smac[i*2+1];
		mac_buf[i][2] = '\0';
	}

	for(i = 0; i < 6; i++)
	{
		cmac[i] = (UCHAR)strtoul((const CHAR*)mac_buf[i], NULL, 16);
	}
}

/* ----------------------------------------------------------------------------
	description : insert semicolon into string mac
	parameters  :
			(const CHAR*) input string MAC
			(CHAR*)       out semicolon inserted mac
			(INT)         out buffer length
	return      : ()
	history     :
		- 2014. 11. 05 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID swc_smac_insert_semicolon(IN const CHAR* smac, OUT CHAR* sout, IN INT nlen)
{
	if((NULL == smac) || (NULL == sout) || (0 == nlen)) return;
	if(12 > strlen(smac)) return;
	snprintf(sout, nlen, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",
		smac[0], smac[1], smac[2], smac[3], smac[4], smac[5], 
		smac[6], smac[7], smac[8], smac[9], smac[10], smac[11]);
}

/* ----------------------------------------------------------------------------
	description : system command 
	parameters  :
			(const CHAR*)    input string MAC
			(UCHAR*) output raw MAC
	return      : (UINT) return CPU count
	history     :
		- 2011. 08. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL swc_system(IN const CHAR* scmd)
{
	pid_t pid;
	int nstatus = 0;
	struct rlimit limit;
	INT i = 0;

	if(NULL == scmd) return FALSE;
	pid = fork();
	swc_sleep(0, 10000);
	if(0 > pid) return FALSE;

	switch(pid)
	{
		case 0 :
		{
			// child process 
			getrlimit(RLIMIT_NOFILE, &limit);
			if(RLIM_INFINITY == limit.rlim_max) limit.rlim_max = 1024;
			for(i = 0; (UINT)i < limit.rlim_max; i++)
			{
				close(i);
			}
			execl("/bin/sh", "sh", "-c", scmd, (CHAR*)0);
			break;
		}
		
		default :
		{
			// parent process 
			while(0 > waitpid(pid, &nstatus, 0))
			{
				if(EINTR != errno)
				return FALSE;
			}
			break;
		}
	}
	return TRUE;
}

/* ----------------------------------------------------------------------------
	description : check if string is number string
	parameters  :
			(const WCString) input string
	return      : (SWC_BOOL) check result
	history     :
		- 2013. 07. 04 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL swc_isnumber_str(IN WCString sNum)
{
	for (INT nID = 0; nID < sNum.GetLength(); nID++)
	{
		if (0 == std::isdigit(sNum[nID])) return FALSE;
	}
	return TRUE;
}

} // end of namespace SWC
//-----------------------------------------------------------------------------
#endif // __SWC_TOOL_CPP__
