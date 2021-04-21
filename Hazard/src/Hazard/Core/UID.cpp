#pragma once

#include <hzrpch.h>
#include "UID.h"
#include <random>

namespace Hazard {

	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	UID::UID() : m_Uid(uniformDistribution(engine)) {}
	UID::UID(uint64_t id) : m_Uid(id) {}
	UID::UID(const UID& other) : m_Uid(other.m_Uid) {}
}
