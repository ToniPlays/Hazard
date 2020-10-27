#pragma once


using namespace Hazard;
class VectorControl {

public:
	static void DrawVec3(const char* label, Vector3<float>& vector, float resetValue = 0.0f, float columnWidth = 100.0f);
	static void DrawVec2(const char* label, Vector2<float>& vector, float resetValue = 0.0f, float columnWidth = 100.0f);
};
