#pragma once
#include <hzrpch.h>
#include "Core/Core.h"

namespace Hazard {

	template <typename T>
	class HAZARD_API Queue {
		public:
			Queue(unsigned int _size) : size(_size) {
				values = std::vector<float>();
				values.resize(size);
			}

			void PushValue(float value) {
				values.push_back(value);
				if (values.size() > size) {
					values.erase(values.begin());
				}
			}
			void Clear() {
				values.clear();
			}
			unsigned int GetSize() { return size; }
			void SetSize(unsigned int _size) {
				size = _size; 
				values.resize(size);
			}
			float* GetData() {
				return values.data();
			}
	private:
		unsigned int size;
		std::vector<float> values;
	};
}