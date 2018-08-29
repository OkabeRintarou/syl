/****************************************************************************
SLog.h

A Simple Log Library for GUI Application


Author : syl
Date : 2014/4/7

*****************************************************************************/
#ifndef __SYL_SLOG_H__
#define __SYL_SLOG_H__

#include <Windows.h>
#include <string>
#include <tchar.h>

template<typename T>
class SLog
{
public:
	static SLog<T> slog;
public:
	SLog(T * filename)
	{
		int namelength = _tcslen(filename);
		_filename = new T[namelength + 1];
		_tcscpy_s(_filename, namelength + 1, filename);

		_filehandle = (HANDLE)CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	}

	~SLog()
	{
		CloseHandle(_filehandle);
		DeleteFile(_filename);
		delete _filename;
	}

	friend SLog<T> & operator<<(SLog<T> & theLog, const T * buffer)
	{
		DWORD dwSize;
		WriteFile(theLog._filehandle, buffer, _tcslen(buffer) * sizeof(T), &dwSize, NULL);
		return theLog;
	}
private:
	HANDLE _filehandle;
	T * _filename;

};

#endif//__SYL_SLOG_H__