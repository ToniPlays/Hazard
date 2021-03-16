#pragma once

namespace NodeEditor {
	class NodeLibrary {
	public:
		static void Reset() { id = 0; }
		static void DrawNode(const char* name);
	private:
		static uint64_t id;
	};
}