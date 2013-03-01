#ifndef SANDBLOX_OSUTILS_H
#define SANDBLOX_OSUTILS_H

#include <string>

#if defined(_WIN32) or defined(_WIN64) or defined(__WINDOWS__)
	#define WINDOWS

	#include <windows.h>
	#include <ctime>
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

	//Simulate stat.h
	#ifdef __GNUC__
		typedef unsigned long long int dev_t;
	#else
		struct dev_t{
			unsigned long __val[2];
		};
	#endif

	struct stat{
		dev_t st_dev;     /* ID of device containing file */
		unsigned long st_ino;     /* inode number */
		unsigned st_mode;    /* protection */
		unsigned st_nlink;   /* number of hard links */
		unsigned st_uid;     /* user ID of owner */
		unsigned st_gid;     /* group ID of owner */
		dev_t st_rdev;    /* device ID (if special file) */
		long st_size;    /* total size, in bytes */
		int st_blksize; /* blocksize for file system I/O */
		unsigned long long st_blocks;  /* number of 512B blocks allocated */
		time_t st_atime;   /* time of last access */
		time_t st_mtime;   /* time of last modification */
		time_t st_ctime;   /* time of last status change */
	};

	int lstat(const char*,stat*);
#endif

#endif
