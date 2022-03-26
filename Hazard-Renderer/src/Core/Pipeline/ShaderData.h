#pragma once

namespace HazardRenderer {

    enum class Optimization { None = 0, Memory, Performance };
    enum class ShaderType;

    struct ShaderStageData;

    struct ShaderDefine {
        std::string Name;
        std::string Value;

        ShaderDefine(std::string name) : Name(name), Value("") {};
        ShaderDefine(std::string name, std::string value) : Name(name), Value(value) {};
    };

    struct CompileInfo
    {
        RenderAPI Environment;
        Optimization Optimization;
        bool CacheResult = true;
        std::string Path = "";
        ShaderType Stage;
        std::string Source;
        std::string Error;
        //Returned by compilation if succeeded
        std::vector<uint32_t> Binary;
        std::vector<ShaderDefine> Defines;

        bool Succeeded() { return Error.empty(); }
    };
}
