project "Hazard-Utility"

	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"examples/**.cpp"
	}

	includedirs
	{
		"src",
        (Dependencies.GLM.IncludeDir),
        (Dependencies.Spdlog.IncludeDir),
        (Dependencies.YAML.IncludeDir),
        (Dependencies.PortableFD.IncludeDir)
	}
 
    References("Spdlog")
    References("YAML")
    References("GLM")
    References("PortableFD")
    
	filter "system:ios"
        files {
            "src/**.mm"
        }
