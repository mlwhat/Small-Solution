#pragma once
#ifndef TIME_H
#define TIME_H
#ifdef _WIN32
#include <windows.h>

namespace simpletime
{
	static LARGE_INTEGER get_frequency()
	{
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		return f;
	}

	static LARGE_INTEGER get_counter()
	{
		LARGE_INTEGER c;
		QueryPerformanceCounter(&c);
		return c;
	}

	LARGE_INTEGER freq;
	LARGE_INTEGER initial;

	void start()
	{
		freq = get_frequency();
		initial = get_counter();
	}
	double milliseconds()
	{
		return (double)(((long double)get_counter().QuadPart - (long double)initial.QuadPart) * 1000 / (long double)freq.QuadPart);
	}
}
#elif  __linux__

#include <sys/time.h>
#include <unistd.h>

namespace mytime
{

	void init()
	{
	}
	double milliseconds()
	{
		struct timeval time;

		gettimeofday(&time, 0);

		return ((double)time.tv_sec) * 1000.0 + ((double)time.tv_usec) / 1000.0;
	}

}
#endif



#endif