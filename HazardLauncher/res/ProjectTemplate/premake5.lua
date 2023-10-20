include "Project/Includes/functional.lua"

ProjectName = "%PROJECT_NAME%"
HazardDirectory = os.getenv("HAZARD_DIR");

workspace "%{ProjectName}"
  architecture "x64"
  targetdir "build"
  startproject "%{ProjectName}"
  dotnetframework "4.7.1"
  

  configurations {
    "Debug",
    "Dist",
    "Release"
  }
  project "%{ProjectName}"
    location "./"
    kind "SharedLib"
    language "C#"

    targetname "%{ProjectName}"
    targetdir "%{prj.location}/Library/Scripts/Binaries"
    objdir "%{prj.location}/Library/Scripts/Obj" 

    defines { _OPTIONS['define'] }

    files {
      "Assets/**.cs"
    }
    links {
      "%{HazardDirectory}/HazardScripting/bin/debug/net7.0/HazardScripting.dll"
    }