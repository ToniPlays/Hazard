#pragma once
#include "Core.h"

namespace Hazard {

	class UID {
	public:
		UID();
		UID(uint64_t id);
		UID(const UID& other);

		operator uint64_t() { return m_Uid; }
		operator const uint64_t() const { return m_Uid; }

	private:
		uint64_t m_Uid = 0;
	};
}
namespace std {
	template<>
	struct hash<Hazard::UID> {

		std::size_t operator() (const Hazard::UID& id) const {
			return hash<uint64_t>()((uint64_t)id);
		}
	};
}
