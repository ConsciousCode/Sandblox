#ifndef SANDBLOX_OSUTILS_H
#define SANDBLOX_OSUTILS_H

#include <string>

#if defined(_WIN32) or defined(_WIN64) or defined(__WINDOWS__)
	#define WINDOWS

	#include <windows.h>
	#include <ctime>

	#include <sys/types.h>
	#include <sys/stat.h>
#elif defined(__linux__) or defined(linux) or defined(__linux) or defined(__gnu_linux__)
	#define LINUX

	#include <dirent.h>
#elif defined(unix) or defined(__unix) or defined(__unix__)
	#define UNIX
#elif defined(__APPLE__) or defined(__MACH__) or defined(macintosh) or defined(Macintosh)
	#define APPLE
#elif defined(__FreeBSD__)
	#define FREEBSD
#endif

#ifdef WINDOWS
	//Simulate dirent.h
	struct dirent{
		TCHAR d_name[MAX_PATH+1];
		WIN32_FIND_DATA data;
	};

	struct DIR{
		char* dirname;
		dirent current;
		int dirent_filled;
		HANDLE search_handle;
	};

	DIR* opendir(const char*);
	dirent* readdir(DIR*);
	int closedir(DIR*);
	void rewinddir(DIR*);
	int _initdir(DIR*);
	const char* _getdirname(const dirent*);
	void _setdirname(DIR*);

	int lstat(const char*,struct stat*);
#endif

#endif
