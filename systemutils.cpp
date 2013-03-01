#include <cstring>

#include "systemutils.h"

#ifdef WINDOWS
	#include <windows.h>

	#include <cstring>
#endif

//#ifdef WINDOWS
	const char* _getdirname(const dirent* dp){
		return dp->data.cFileName;
	}

	void _setdirname(DIR* dirp){
		strncpy(dirp->current.d_name,_getdirname(&dirp->current),MAX_PATH);
		dirp->current.d_name[NAME_MAX] = '\0'; /*char d_name[NAME_MAX+1]*/
	}

	int _initdir(DIR* dirp){
		dirp->dirent_filled=0;
		dirp->search_handle=FindFirstFile(dir->dirname,&dirp->current.data);
		if(dirp->search_handle==INVALID_HANDLE_VALUE){
			return 0;
		}
		_setdirname(dirp);
		dirp->dirent_filled=1;
		return 1;
	}

	DIR* opendir(const char* dirname){
		if(!dirname){
			return NULL;
		}

		DIR* dirp=new DIR;
		//4=strlen("\\*.*")
		dirp->dirname=new char[strlen(dirname)+4+1];
		strcpy(dirp->dirname,dirname);
		char* p=strchr(dirp->dirname,'\0');
		if(dirp->dirname<p and *(p-1)!='\\' and *(p-1)!='/' and *(p-1)!=':'){
		  strcpy(p++,"\\");
		}
		strcpy(p,"*");
		if(!_initdir(dirp)){
			delete[] dir->dirname;
			delete dirp;
			return NULL;
		}
		return dirp;
	}

	dirent* readdir(DIR* dirp){
		if(!dirp){
			return NULL;
		}
		if(dirp->search_handle==INVALID_HANDLE_VALUE){
			return NULL;
		}
		if(!dirp->dirent_filled){
			dirp->dirent_filled=0;
		}
		else{
			if(!FindNextFile(dirp->search_handle,&dirp->current.data)){
				FindClose(dirp->search_handle);
				dirp->search_handle=INVALID_HANDLE_VALUE;
				return NULL;
			}
			_setdirname(dirp);
		}
		return &dirp->current;
	}

	int closedir(DIR* dirp){
		if(!dirp){
			return -1;
		}

		int retcode=0;

		if(dirp->dirname){
			delete dirp->dirname;
		}
		if(dirp->search_handle!=INVALID_HANDLE_VALUE){
			if(!FindClose(dirp->search_handle)){
				retcode=-1;
			}
		}

		delete dirp;

		return retcode;
	}

	void rewinddir(DIR* dirp){
		if(!dirp){
			return;
		}
		if(!dirp->dirname){
			return;
		}
		if(dirp->search_handle!=INVALID_HANDLE_VALUE){
			if(!FindClose(dirp->search_handle)){
				return;
			}
		}
		_initdir(dirp);
	}

	static time_t ft2tt(FILETIME const& ft){
		ULARGE_INTEGER ull;
		ull.LowPart=ft.dwLowDateTime;
		ull.HighPart=ft.dwHighDateTime;
		return ull.QuadPart/10000000ULL-11644473600ULL;
	}

	int lstat(const char* path,stat* buf){
		HANDLE file=CreateFile(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,SECURITY_IMPERSONATION,NULL);

		if(file==INVALID_HANDLE_VALUE){
			return -1;
		}

		BY_HANDLE_FILE_INFORMATION bhfi;

		if(!GetFileInformationByHandle(file,&bhfi)){
			CloseFile(file);
			return -1;
		}
		CloseFile(file);

		((long*)buf->st_dev)[0]=bhfi.nFileIndexHigh;
		((long*)buf->st_dev)[1]=bhfi.nFileIndexLow;

		buf->st_ctime=ft2tt(bhfi.ftCreationTime);
		buf->st_atime=ft2tt(bhfi.ftLastAccessTime);
		buf->st_mtime=ft2tt(bhfi.ftLastWriteTime);

		buf->st_nlink=bhfi.nNumberOfLinks;

		buf->st_size=bhfi.nFileSizeLow;

		return 0;
	}

#endif
