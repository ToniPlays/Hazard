#pragma once

#include "Hazard.h"

struct Message {
	int level = 0;
	std::string text;
	std::string time;
};

class Debug {

public:
	static void Log(std::string text);
	static void Warn(std::string text);
	static void Error(std::string text);
	static void Clear() { logs.clear(); }
	static std::vector<Message*> GetAll() { return logs; }
private:
	static std::vector<Message*> logs;
};