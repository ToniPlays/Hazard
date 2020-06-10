#pragma once
#include "Hazard.h"
#include <imgui.h>
#include <ctime>

inline ImVec4 ColorAsImVec(Hazard::Color color) {
	return ImVec4(color.r, color.g, color.b, color.a);
}
inline std::string CurrentTime() {
	time_t now = time(0);
	char timestamp[10] = "";
	strftime(timestamp, 10, "%H:%M:%S", localtime(&now));
	return timestamp;
}
inline ImVec2 GetMaxSize(ImVec2 size, int w, int h) {

	/*float sizeRatio = size.x / size.y;
	float wRatio = (float)w / (float)h;
	float hRatio = (float)h / (float)w;
	std::cout << sizeRatio << std::endl;

	if (sizeRatio >= wRatio) {
		
		return ImVec2(hRatio * (float)w, size.y);
	}
	else {
		return ImVec2(960, 540);
		
	}*/
	return ImVec2(w, h);
}