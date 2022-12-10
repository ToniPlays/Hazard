from distutils import file_util
import os
import sys
import subprocess
import platform
from pathlib import Path

import Utils
import FileUtils

from io import BytesIO
from urllib.request import urlopen

class VulkanConfiguration:
    requiredVulkanVersion = ""
    requiredDebugLibVersion = ""
    vulkanDirectory = "../Hazard/vendor/VulkanSDK"
    vulkanDownloadURL = "https://sdk.lunarg.com/sdk/download"

    @classmethod
    def Validate(cls, version):

        cls.requiredVulkanVersion = version
        cls.requiredDebugLibVersion = version

        if (not cls.CheckVulkanSDK()):
            return False

        return True

    @classmethod
    def CheckVulkanSDK(cls):
        vulkanSDK = os.environ.get("VULKAN_SDK")

        if (vulkanSDK is None):
            return False

        if (cls.requiredVulkanVersion not in vulkanSDK):
            return False

        return True

    @classmethod
    def InstallVulkanSDK(cls):
        platformExecutable = Utils.PlatformExecutable()

        if Utils.IsWindows():
            vulkanInstallURL = f"{cls.vulkanDownloadURL}/{cls.requiredVulkanVersion}/windows/VulkanSDK-{cls.requiredVulkanVersion}-Installer.exe"
        elif Utils.IsMacos():
            vulkanInstallURL = f"{cls.vulkanDownloadURL}/{cls.requiredVulkanVersion}/mac/vulkansdk-macos-{cls.requiredVulkanVersion}.dmg"
           
        
        vulkanPath = f"{cls.vulkanDirectory}/VulkanSDK-{cls.requiredVulkanVersion}-Installer.{platformExecutable}";
        print("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        FileUtils.DownloadFile(vulkanInstallURL, vulkanPath)
        print("Running Vulkan SDK installer...")
        
        
        if Utils.IsWindows():
            os.startfile(os.path.abspath(vulkanPath))
        else:
            opener = "open" if Utils.IsMacos() else "xdg-open"
            subprocess.call([opener, os.path.abspath(vulkanPath)])

        print("Re-run this script after Vulkan installation!")
        quit()

    @classmethod
    def CheckVulkanSDKDebugLibs(cls):
        shadercdLib = Path(f"{cls.vulkanDirectory}/Lib/shaderc_sharedd.lib")

        VulkanSDKDebugLibsURLlist = [
            f"https://sdk.lunarg.com/sdk/download/{cls.requiredVulkanVersion}/windows/VulkanSDK-{cls.requiredVulkanVersion}-DebugLibs.zip",
            f"https://files.lunarg.com/SDK-{cls.requiredVulkanVersion}/VulkanSDK-{cls.requiredVulkanVersion}-DebugLibs.zip"
        ]

        if not shadercdLib.exists():
            print(f"\nNo Vulkan SDK debug libs found. (Checked {shadercdLib})")
            vulkanPath = f"{cls.vulkanDirectory}/VulkanSDK-{cls.requiredVulkanVersion}-DebugLibs.zip"
            FileUtils.DownloadFile(VulkanSDKDebugLibsURLlist, vulkanPath)
            print("Extracting", vulkanPath)
            FileUtils.UnzipFile(vulkanPath, deleteZipFile=False)
            print(f"Vulkan SDK debug libs installed at {os.path.abspath(cls.vulkanDirectory)}")
        else:
            print(f"Vulkan SDK debug libs located at {os.path.abspath(cls.vulkanDirectory)}")
        return True

if __name__ == "__main__":
    VulkanConfiguration.Validate()
