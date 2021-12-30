#pragma once
#include <hzrpch.h>
#include "GUI/EditorWindow.h"

namespace WindowElement {

	struct ProfileSessionData 
	{
		std::string name;
		uint32_t duration;
	};

	class Profiler : public EditorWindow {
	public:
		Profiler();
		~Profiler() = default;

		void Init() override;
		void OnWindowRender() override;
	private:
		void GenerateSequence(const std::string& file);
		std::vector<ProfileSessionData> m_Data;
	};
}