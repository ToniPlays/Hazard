
#include "UUID.h"

namespace Hazard {

	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	UUID::UUID() : m_Uid(uniformDistribution(engine)) {}
	UUID::UUID(uint64_t id) : m_Uid(id) {}
	UUID::UUID(const UUID& other) : m_Uid(other.m_Uid) {}
}
