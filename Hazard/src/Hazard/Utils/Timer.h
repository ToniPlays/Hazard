#pragma once
#include <chrono>
#include <iostream>

using namespace std;

class Timer {
public:
	Timer() {
		start = chrono::high_resolution_clock::now();
	}
	~Timer() {
		Stop();
	}
	void Stop() {
		auto curTime = chrono::high_resolution_clock::now();
		auto endTime = chrono::time_point_cast<chrono::microseconds>(curTime).time_since_epoch().count();
		auto startTime = chrono::time_point_cast<chrono::microseconds>(start).time_since_epoch().count();

		auto duration = endTime - startTime;

		cout << duration * 0.001f << "ms" << endl;
	}
private:
	chrono::time_point<chrono::high_resolution_clock> start;
};