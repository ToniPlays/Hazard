#pragma once

#include "Buffer/Buffer.h"
#include "Core/ShaderCompiler.h"

namespace HazardRenderer::Metal
{
    struct MSLBinding {
        uint32_t Binding;
        uint32_t SamplerBinding;
    };
    #ifdef HZR_DESKTOP

    class MetalShaderCompiler
    {
    public:
        MetalShaderCompiler() = default;
        ~MetalShaderCompiler();

        bool Compile(CompileInfo* compileInfo);
        bool Decompile(Buffer binary, std::string& result, bool tesellation = false);
        std::unordered_map<std::string, MSLBinding> GetMSLBindings(Buffer binary, bool tesellation = false);

        double GetCompileTime() { return m_CompilationTime; }
        std::string GetErrorMessage() { return m_ErrorMessage; }
        Buffer GetCompiledBinary() { return m_ResultBinary; }

    private:
        Buffer m_ResultBinary;
        std::string m_ErrorMessage;
        double m_CompilationTime = 0.0;
    };
    #endif
}
