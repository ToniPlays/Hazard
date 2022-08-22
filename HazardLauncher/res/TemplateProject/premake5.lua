
ProjectName = "%PROJECT_NAME%"
HazardDirectory = os.getenv("HAZARD_DIR");

include "Library/Linker.lua"

workspace "%{ProjectName}"
  architecture "x64"
  targetdir "build"
  startproject "%{ProjectName}"
  

  configurations {
    "Debug",
    "Dist",
    "Release"
  }

  project "%{ProjectName}"
    location "./"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetname "%{ProjectName}"
    targetdir "%{prj.location}/Library/Scripts/Binaries"
    objdir "%{prj.location}/Library/Scripts/Obj" 

    files {
      "Assets/**.cs"
    }
    links 
    {
      "%{HazardDirectory}/HazardScripting/bin/debug/HazardScripting.dll"
    }

    linkAppReferences()
