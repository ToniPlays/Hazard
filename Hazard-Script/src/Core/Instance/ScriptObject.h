#pragma once

#include "Ref.h"
#include "Coral/ManagedObject.hpp"

namespace HazardScript
{
	class ScriptMetadata;

	class ScriptObject : public RefCount
	{
		friend class ScriptMetadata;
	public:
		ScriptObject() = default;
		~ScriptObject();

		template<typename... Args>
		void TryInvoke(const std::string& name, Args&&... params)
		{
			/*auto& methods = m_Handle.GetType().GetMethods();

			for (auto& methodInfo : methods)
			{
				if (methodInfo.Name.ToString() != name) continue;
				return;
			}*/
			m_Handle.InvokeMethod(name, std::forward<Args>(params)...);
		}

		template<typename TRet, typename... Args>
		TRet Invoke(const std::string& name, Args&&... params)
		{
			return m_Handle.InvokeMethod<TRet>(name, std::forward<Args>(params)...);
		}
		template<typename... Args>
		void Invoke(const std::string& name, Args&&... params)
		{
			m_Handle.InvokeMethod(name, std::forward<Args>(params)...);
		}

		template<typename T>
		T GetFieldValue(const std::string& name, uint32_t index = 0)
		{
			return m_Handle.GetFieldValue<T>(name);
		}
		template<typename T>
		void SetFieldValue(const std::string& name, T value, uint32_t index = 0)
		{
			m_Handle.SetFieldValue(name, value);
		}

		void SetArraySize(const std::string& name, uint32_t elements)
		{
			//m_Script->SetArraySize(m_Handle, name, elements);
		}
		void SetLive(bool live) { m_IsLive = live; }

	private:
		Coral::ManagedObject m_Handle;
		bool m_IsLive = false;
	};
}
