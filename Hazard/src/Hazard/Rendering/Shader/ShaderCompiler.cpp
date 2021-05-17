#pragma once

#include <hzrpch.h>
#include "ShaderCompiler.h"
#include "Hazard/File/File.h"

#include "spirv-tools/libspirv.hpp"
#include "spirv-tools/optimizer.hpp"

namespace Hazard::Rendering 
{
	bool ShaderCompiler::CompileShader(const std::string& sourceFile, const std::string& dest)
	{
		HZR_CORE_INFO("Compiling shader to SPIR-V binary");
		HZR_CORE_INFO("Input file: " + sourceFile);
		HZR_CORE_INFO("Output file " + dest);

		std::string& source = File::ReadFile(sourceFile);

		spvtools::SpirvTools core(SPV_ENV_UNIVERSAL_1_1);
		//std::vector<uint32_t> binary;

		//if (!core.Assemble(source, &binary))
		{
			HZR_THROW("Failed to assemble Shader");
		}

		return true;
	}
}