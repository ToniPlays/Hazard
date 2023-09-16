#pragma once

#include "Ref.h"

namespace HazardScript 
{
	class FieldValueStorageBase : public RefCount
	{
	public:
		void SetLive(bool isLive)
		{
			m_IsLiveValue = isLive;
		};

		bool IsLive() { return m_IsLiveValue; }
		//virtual Buffer GetValueOrDefault(MonoObject* object) = 0;
		//virtual void SetData(MonoObject* object, uint32_t index, Buffer value) {};

	protected:
		bool m_IsLiveValue = false;
	};
}
