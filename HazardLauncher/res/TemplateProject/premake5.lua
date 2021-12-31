
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
    location "Assets/Scripts"
    kind "SharedLib"
    language "C#"

    targetname "%{ProjectName}"
    targetdir "%{prj.location}/Binaries"

    files {
      "Assets/Scripts/**.cs"
    }
    links {
      "%{HazardDirectory}/HazardScripting/bin/debug/HazardScripting.dll"
    }
