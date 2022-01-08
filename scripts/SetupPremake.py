import sys
import os
import platform
from pathlib import Path

import Utils

class PremakeConfiguration:
    premakeVersion = "5.0.0-alpha16"
    premakePlatform = ""
    premakeZipUrls = f"https://github.com/premake/premake-core/releases/download/"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./vendor/premake/bin"

    @classmethod
    def Validate(cls):
        if platform.system() == "Windows":
            cls.premakeVersion = "5.0.0-beta1"
            cls.premakePlatform = f"-windows.zip"
        elif platform.system() == "Darwin":
            cls.premakeVersion = "5.0.0-beta1"
            cls.premakePlatform = f"-macosx.tar.gz"
                        
        if (not cls.CheckIfPremakeInstalled()):
            print("Premake is not installed.")
            return False

        print(f"Correct Premake located at {os.path.abspath(cls.premakeDirectory)}")
        return True

    @classmethod
    def CheckIfPremakeInstalled(cls):
        premakeApp = Path("")
        if platform.system() == "Windows":
            premakeApp = Path(f"{cls.premakeDirectory}/premake5.exe");
        elif platform.system() == "Darwin":
            premakeApp = Path(f"{cls.premakeDirectory}/premake5");

        if (not premakeApp.exists()):
            return cls.InstallPremake()

        return True

    @classmethod
    def InstallPremake(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Premake not found. Would you like to download Premake {0:s}? [Y/N]: ".format(cls.premakeVersion))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')

        premakePath = f"{cls.premakeDirectory}/premake-{cls.premakeVersion}{cls.premakePlatform}"
        
        premakeUrl = cls.premakeZipUrls + "v" + cls.premakeVersion + "/premake-" + cls.premakeVersion + cls.premakePlatform
        print("Downloading {0:s} to {1:s}".format(premakeUrl, premakePath))
        Utils.DownloadFile(premakeUrl, premakePath)
        
        print("Extracting", premakePath)
        if platform.system() == "Windows":
            Utils.UnzipFile(premakePath, deleteZipFile=True)
        elif platform.system() == "Darwin":
            Utils.UnzipTar(premakePath, deleteZipFile=True)
        print(f"Premake {cls.premakeVersion} has been downloaded to '{cls.premakeDirectory}'")

        premakeLicensePath = f"{cls.premakeDirectory}/LICENSE.txt"
        print("Downloading {0:s} to {1:s}".format(cls.premakeLicenseUrl, premakeLicensePath))
        Utils.DownloadFile(cls.premakeLicenseUrl, premakeLicensePath)
        print(f"Premake License file has been downloaded to '{cls.premakeDirectory}'")

        return True
