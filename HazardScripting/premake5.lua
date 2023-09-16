project "HazardScripting"
	kind "SharedLib"
	language "C#"
	dotnetframework "net7.0"
	clr "unsafe"

	files
	{
		"source/**.cs"
	}

	includedirs
	{
		"src"
	}
