#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>


struct Message {
	int level = 0;
	std::string text;
};

class Debug {

public:
	static void Log(std::string text);
	static void Warn(std::string text);
	static void Error(std::string text);
	void Clear() { logs.clear(); }
	std::vector<Message*> getAll() { return logs; }
private:
	static std::vector<Message*> logs;
};