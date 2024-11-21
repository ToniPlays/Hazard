project "HazardScripting"
	kind "SharedLib"
	language "C#"
	dotnetframework "net8.0"
	clr "unsafe"

	files
	{
		"source/**.cs"
	}

	includedirs
	{
		"src"
	}
	links
	{
		"Coral.Managed"
	}
