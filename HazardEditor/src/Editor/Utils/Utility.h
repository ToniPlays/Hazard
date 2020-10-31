#pragma once
#include <hzreditor.h>
#include "Hazard.h"


inline ImVec4 ColorAsImVec(Hazard::Color color) {
	return ImVec4(color.r, color.g, color.b, color.a);
}
inline const char* CurrentTime() {
	time_t now = time(0);
	char timestamp[10] = "";
	strftime(timestamp, 10, "%H:%M:%S", localtime(&now));
	return timestamp;
}
