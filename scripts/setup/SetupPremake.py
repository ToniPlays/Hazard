import os
import platform
from pathlib import Path

import Utils
import FileUtils

class PremakeConfiguration:
    premakeVersion = "5.0.0-alpha16"
    premakePlatform = ""
    premakeZipUrls = f"https://github.com/premake/premake-core/releases/download/"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./vendor/premake/bin"

    @classmethod
    def Validate(cls, version):
        cls.premakeVersion = version

        if Utils.IsWindows():
            cls.premakePlatform = f"-windows.zip"
        elif Utils.IsMacos():
            cls.premakePlatform = f"-macosx.tar.gz"
                        
        if (not cls.CheckIfPremakeInstalled()):
            return False

        return True

    @classmethod
    def CheckIfPremakeInstalled(cls):
        premakeApp = Path("")
        if Utils.IsWindows():
            premakeApp = Path(f"{cls.premakeDirectory}/premake5.exe");
        elif Utils.IsMacos():
            premakeApp = Path(f"{cls.premakeDirectory}/premake5");

        return premakeApp.exists()

    @classmethod
    def InstallPremake(cls):
        
        premakePath = f"{cls.premakeDirectory}/premake-{cls.premakeVersion}{cls.premakePlatform}"
        
        premakeUrl = cls.premakeZipUrls + "v" + cls.premakeVersion + "/premake-" + cls.premakeVersion + cls.premakePlatform
        print("Downloading {0:s} to {1:s}".format(premakeUrl, premakePath))
        FileUtils.DownloadFile(premakeUrl, premakePath)
        
        print("Extracting", premakePath)
        if Utils.IsWindows():
            FileUtils.UnzipFile(premakePath, deleteZipFile=True)
        elif Utils.IsMacos():
            FileUtils.UnzipTar(premakePath, deleteZipFile=True)
        print(f"Premake {cls.premakeVersion} has been downloaded to '{cls.premakeDirectory}'")

        premakeLicensePath = f"{cls.premakeDirectory}/LICENSE.txt"
        print("Downloading {0:s} to {1:s}".format(cls.premakeLicenseUrl, premakeLicensePath))
        FileUtils.DownloadFile(cls.premakeLicenseUrl, premakeLicensePath)
        print(f"Premake License file has been downloaded to '{cls.premakeDirectory}'")

        return True
