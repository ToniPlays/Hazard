
ProjectName = "%PROJECT_NAME%"
HazardDirectory = os.getenv("HAZARD_DIR");

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

    targetname "%{ProjectName}"
    targetdir "%{prj.location}/Library/Scripts/Binaries"
    objdir "%{prj.location}/Library/Scripts/Obj" 

    files {
      "Assets/**.cs"
    }
    links {
      "%{HazardDirectory}/HazardScripting/bin/debug/HazardScripting.dll"
    }
