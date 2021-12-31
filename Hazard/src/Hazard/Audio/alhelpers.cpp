#pragma once

#include <hzrpch.h>
#include "alhelpers.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

// #include "AL/al.h"

// int InitAL(ALCdevice*& device, char*** argv, int* argc)
// {
// 	ALCcontext* ctx;

// 	device = NULL;
// 	if (argc && argv && *argc > 1 && strcmp((*argv)[0], "-device") == 0)
// 	{
// 		device = alcOpenDevice((*argv)[1]);
// 		if (!device)
// 			fprintf(stderr, "Failed to open \"%s\", trying default\n", (*argv)[1]);
// 		(*argv) += 2;
// 		(*argc) -= 2;
// 	}
// 	if (!device)
// 		device = alcOpenDevice(NULL);
// 	if (!device)
// 	{
// 		fprintf(stderr, "Could not open a device!\n");
// 		return 1;
// 	}

// 	ctx = alcCreateContext(device, NULL);
// 	if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
// 	{
// 		if (ctx != NULL)
// 			alcDestroyContext(ctx);
// 		alcCloseDevice(device);
// 		fprintf(stderr, "Could not set a context!\n");
// 		return 1;
// 	}

// 	return 0;
// }

// void CloseAL(void)
// {
// 	ALCdevice* device;
// 	ALCcontext* ctx;

// 	ctx = alcGetCurrentContext();
// 	if (ctx == NULL)
// 		return;

// 	device = alcGetContextsDevice(ctx);

// 	alcMakeContextCurrent(NULL);
// 	alcDestroyContext(ctx);
// 	alcCloseDevice(device);
// }


// const char* FormatName(ALenum format)
// {
// 	switch (format)
// 	{
// 	case AL_FORMAT_MONO8: return "Mono, U8";
// 	case AL_FORMAT_MONO16: return "Mono, S16";
// 	case AL_FORMAT_STEREO8: return "Stereo, U8";
// 	case AL_FORMAT_STEREO16: return "Stereo, S16";
// 	}
// 	return "Unknown Format";
// }


#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

int altime_get(void)
{
	static int start_time = 0;
	int cur_time;
	union {
		FILETIME ftime;
		ULARGE_INTEGER ulint;
	} systime;
	GetSystemTimeAsFileTime(&systime.ftime);
	cur_time = (int)(systime.ulint.QuadPart / 10000);

	if (!start_time)
		start_time = cur_time;
	return cur_time - start_time;
}

void al_nssleep(unsigned long nsec)
{
	Sleep(nsec / 1000000);
}

#else

#include <sys/time.h>
#include <unistd.h>
#include <time.h>

// int altime_get(void)
// {
// 	static int start_time = 0u;
// 	int cur_time;

// #if _POSIX_TIMERS > 0
// 	struct timespec ts;
// 	int ret = clock_gettime(CLOCK_REALTIME, &ts);
// 	if (ret != 0) return 0;
// 	cur_time = (int)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
// #else /* _POSIX_TIMERS > 0 */
// 	struct timeval tv;
// 	int ret = gettimeofday(&tv, NULL);
// 	if (ret != 0) return 0;
// 	cur_time = (int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
// #endif

// 	if (!start_time)
// 		start_time = cur_time;
// 	return cur_time - start_time;
// }

// void al_nssleep(unsigned long nsec)
// {
// 	struct timespec ts, rem;
// 	ts.tv_sec = (time_t)(nsec / 1000000000ul);
// 	ts.tv_nsec = (long)(nsec % 1000000000ul);
// 	while (nanosleep(&ts, &rem) == -1 && errno == EINTR)
// 		ts = rem;
// }

#endif