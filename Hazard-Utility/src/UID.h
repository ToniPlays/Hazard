#pragma once

#include <ios>
#include <random>

namespace HazardUtility {

	class UID {
	public:
		UID();
		UID(uint64_t id);
		UID(const HazardUtility::UID& other);

		operator uint64_t() { return m_Uid; }
		operator const uint64_t() const { return m_Uid; }

	private:
		uint64_t m_Uid;
	};
}
namespace std {
	template<>
	struct hash<HazardUtility::UID> {

		std::size_t operator() (const HazardUtility::UID& id) const {
			return hash<uint64_t>()((uint64_t)id);
		}
	};
}
