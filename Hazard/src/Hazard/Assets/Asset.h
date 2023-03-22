#pragma once

#include "Ref.h"
#include "UID.h"
#include "AssetEnums.h"

#include "UtilityCore.h"

#include <filesystem>
#include <unordered_map>

#define INVALID_ASSET_HANDLE (AssetHandle)0

namespace Hazard
{
	enum class LoadState { None, Loading, Loaded };

	enum class AssetFlags : uint16_t
	{
		None = BIT(0),
		Valid = BIT(1),
		Missing = BIT(2),
		Invalid = BIT(3),
		InvalidInput = BIT(4),
		MemoryOnly = BIT(5)
	};

	using AssetHandle = UID;

	struct AssetMetadata
	{
		AssetHandle Handle = INVALID_ASSET_HANDLE;
		AssetType Type = AssetType::Undefined;
		LoadState LoadState = LoadState::None;

		std::string Key = "";
		std::filesystem::path AssetPackFile = "";

		bool MemoryOnly = false;

		bool IsValid() { return Handle && Type != AssetType::Undefined; }
	};

	class Asset : public RefCount
	{
		friend class AssetManager;
		friend class ImageAssetLoader;
		friend class WorldAssetLoader;
	public:

		virtual ~Asset();

		const AssetType GetType() const { return m_Type; }
		AssetHandle GetHandle() const { return m_Handle; }
		uint32_t GetFlags() const { return m_Flags; }
		bool IsValid() const { return m_Handle != INVALID_ASSET_HANDLE && m_Type != AssetType::Undefined; }

	protected:
		AssetHandle m_Handle = INVALID_ASSET_HANDLE;
		AssetType m_Type = AssetType::Undefined;
		uint16_t m_Flags = 0;

	private:
		void SetHandle(UID handle) { m_Handle = handle; };
		void SetFlags(uint32_t flags) { m_Flags = flags; }
	};

	class AssetPointer : public Asset
	{
	public:
		Ref<RefCount> Value = nullptr;

		static Ref<AssetPointer> Create(Ref<RefCount> value, AssetType type)
		{
			Ref<AssetPointer> pointer = Ref<AssetPointer>::Create();
			pointer->Value = value;
			pointer->m_Type = type;

			return pointer;
		}
	};
}
