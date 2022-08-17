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

	protected:
		bool m_IsLiveValue = false;
	};
}