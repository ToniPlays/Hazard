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
	};

	using AssetHandle = UID;

	struct AssetMetadata
	{
		AssetHandle AssetPackHandle = INVALID_ASSET_HANDLE;
		AssetHandle Handle = INVALID_ASSET_HANDLE;

		AssetType Type = AssetType::Undefined;
		LoadState LoadState = LoadState::None;
		std::filesystem::path FilePath = "";	//Can be whatever
		std::filesystem::path SourceFile = "";	//Original asset location

		bool IsValid() { return Handle && Type != AssetType::Undefined; }

		bool operator==(const AssetMetadata& other) const {
			return Handle == other.Handle && SourceFile == other.SourceFile;
		}
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
		bool IsValid() const;

	private:
		void SetHandle(UID handle) { m_Handle = handle; };
		void SetFlags(uint32_t flags) { m_Flags = flags; }

	protected:
		AssetHandle m_Handle = INVALID_ASSET_HANDLE;
		uint16_t m_Flags = 0;
		std::filesystem::path m_SourceAssetPath = "";
	};

	class AssetPointer : public Asset
	{
	public:
		AssetPointer() = default;

		Ref<RefCount> Value = nullptr;

		AssetType GetType() const override { return AssetType::Undefined; }

		static Ref<AssetPointer> Create(Ref<RefCount> value, AssetType type);
	};
}

namespace std
{
	template <>
	struct hash<Hazard::AssetMetadata>
	{
		std::uint64_t operator()(const Hazard::AssetMetadata& metadata) const
		{
			return hash<uint64_t>()((uint64_t)metadata.Handle);
		}
	};
}
