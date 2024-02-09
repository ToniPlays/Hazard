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
		AssetHandle AssetPackHandle = INVALID_ASSET_HANDLE;

		AssetType Type = AssetType::Undefined;
		LoadState LoadState = LoadState::None;
		std::filesystem::path FilePath = "";	//Can be whatever
		std::filesystem::path SourceFile = "";	//Original asset location

		bool IsValid() { return Handle && Type != AssetType::Undefined; }
	};

	class Asset : public RefCount
	{
		friend class AssetManager;
		friend class AssetLoader;

	public:
		virtual ~Asset();

		virtual AssetType GetType() const = 0;
		AssetHandle GetHandle() const { return m_Handle; }
		uint32_t GetFlags() const { return m_Flags; }
		const std::filesystem::path& GetSourceFilePath() const { return m_SourceAssetPath; }
		void SetSourceFilePath(const std::filesystem::path& newPath) { m_SourceAssetPath = newPath; }
		bool IsValid() const { return m_Handle != INVALID_ASSET_HANDLE && GetType() != AssetType::Undefined; }

	protected:
		AssetHandle m_Handle = INVALID_ASSET_HANDLE;
		uint16_t m_Flags = 0;
		std::filesystem::path m_SourceAssetPath = "";

	private:
		void SetHandle(UID handle) { m_Handle = handle; };
		void SetFlags(uint32_t flags) { m_Flags = flags; }
	};

	class AssetPointer : public Asset
	{
	public:
		AssetPointer() = default;

		Ref<RefCount> Value = nullptr;

		AssetType GetType() const override { return AssetType::Undefined; }

		static Ref<AssetPointer> Create(Ref<RefCount> value, AssetType type)
		{
			Ref<AssetPointer> pointer = Ref<AssetPointer>::Create();
			pointer->Value = value;

			return pointer;
		}
	};
}
