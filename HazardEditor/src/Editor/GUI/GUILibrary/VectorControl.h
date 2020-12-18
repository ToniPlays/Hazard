#pragma once


using namespace Hazard;
class VectorControl {

public:
	static void DravVec1(const char* label, float& value, float resetValue = 0.0f, float columnWidth = 100.0f);
	static void DrawVec2(const char* label, glm::vec2& vector, float resetValue = 0.0f, float columnWidth = 100.0f);
	static void DrawVec3(const char* label, glm::vec3& vector, float resetValue = 0.0f, float columnWidth = 100.0f);
};
