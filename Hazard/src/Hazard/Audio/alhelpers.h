#ifndef ALHELPERS_H
#define ALHELPERS_H

#include "AL/al.h"
#include "AL/alc.h"

#ifdef __cplusplus
extern "C" {
#endif

	const char* FormatName(ALenum type);

	int InitAL(ALCdevice*& device, char*** argv, int* argc);
	void CloseAL(void);

	int altime_get(void);
	void al_nssleep(unsigned long nsec);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ALHELPERS_H */