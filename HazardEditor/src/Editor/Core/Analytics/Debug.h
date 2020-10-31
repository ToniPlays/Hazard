#pragma once
#include "Hazard.h"

/*struct Message {
	int level = 0;
	const char* text;
	const char* time;
};*/

class Debug {

public:
	static void Log(std::string text);
	static void Warn(std::string text);
	static void Error(std::string text);
private:
};