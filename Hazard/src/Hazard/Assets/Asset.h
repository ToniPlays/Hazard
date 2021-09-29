#pragma once

#include "Hazard/Core/Ref.h"
#include "Hazard/Core/UUID.h"
#include "AssetEnums.h"

#define INVALID_ASSET_HANDLE 0

namespace Hazard 
{
	enum class AssetFlags : uint16_t {
		None			= -1,
		Valid			= BIT(0),
		Missing			= BIT(1),
		Invalid			= BIT(2),
		InvalidInput	= BIT(3)
	};

	using AssetHandle = UUID;

	class Asset : public RefCount 
	{
		friend class AssetManager;
	public:

		virtual ~Asset() = default;
		const AssetType GetType() const { return m_Type; }
		UUID GetHandle() { return m_Handle; }
		AssetFlags GetFlags() { return m_Flags; }
	
	protected:
		UUID m_Handle;
		AssetType m_Type;
		AssetFlags m_Flags;

	private:
		void SetHandle(UUID handle) { m_Handle = handle; };
		void SetFlags(AssetFlags flags) { m_Flags = flags; }
	};
}