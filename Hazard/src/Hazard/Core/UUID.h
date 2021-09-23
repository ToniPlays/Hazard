#pragma once
#include "Core.h"

namespace Hazard {

	class UUID {
	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID& other);

		operator uint64_t() { return m_Uid; }
		operator const uint64_t() const { return m_Uid; }

	private:
		uint64_t m_Uid = 0;
	};
}
namespace std {
	template<>
	struct hash<Hazard::UUID> {

		std::size_t operator() (const Hazard::UUID& id) const {
			return hash<uint64_t>()((uint64_t)id);
		}
	};
}
