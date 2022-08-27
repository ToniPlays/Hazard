import os
import sys
import subprocess
import platform
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import urlopen



platform = "windows" if platform.system() == "Windows" else "mac";
platformExecutable = "exe" if platform == "Windows" else "dmg";


class VulkanConfiguration:
    requiredVulkanVersion = "1.3.204.1"
    requiredDebugLibVersion = "1.3.204.1";
    vulkanDirectory = "../Hazard/vendor/VulkanSDK"

    @classmethod
    def Validate(cls):
        if (not cls.CheckVulkanSDK()):
            print("Vulkan SDK not installed correctly.")
            return

        if (not cls.CheckVulkanSDKDebugLibs()):
            print("Vulkan SDK debug libs not found.")

    @classmethod
    def CheckVulkanSDK(cls):
        vulkanSDK = os.environ.get("VULKAN_SDK")
        if (vulkanSDK is None):
            print("\nYou don't have the Vulkan SDK installed!")
            cls.__InstallVulkanSDK()
            return False
        else:
            print(f"\nLocated Vulkan SDK at {vulkanSDK}")

        if (cls.requiredVulkanVersion not in vulkanSDK):
            print(f"You don't have the correct Vulkan SDK version! (Engine requires {cls.requiredVulkanVersion})")
            cls.__InstallVulkanSDK()
            return False

        print(f"Correct Vulkan SDK located at {vulkanSDK}")
        return True

    @classmethod
    def __InstallVulkanSDK(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(cls.requiredVulkanVersion))).lower().strip()[:1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        if platform == "windows":
            vulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{cls.requiredVulkanVersion}/{platform}/VulkanSDK-{cls.requiredVulkanVersion}-Installer.exe"
        else:
            vulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{cls.requiredVulkanVersion}/mac/vulkansdk-macos-{cls.requiredVulkanVersion}.dmg"
        
        
        vulkanPath = f"{cls.vulkanDirectory}/VulkanSDK-{cls.requiredVulkanVersion}-Installer.{platformExecutable}";
        print("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        Utils.DownloadFile(vulkanInstallURL, vulkanPath)
        print("Running Vulkan SDK installer...")
        
        
        if platform == "windows":
            os.startfile(os.path.abspath(vulkanPath))
        else:
            opener = "open" if sys.platform == "darwin" else "xdg-open"
            subprocess.call([opener, os.path.abspath(vulkanPath)])
        print("Re-run this script after installation!")
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
            Utils.DownloadFile(VulkanSDKDebugLibsURLlist, vulkanPath)
            print("Extracting", vulkanPath)
            Utils.UnzipFile(vulkanPath, deleteZipFile=False)
            print(f"Vulkan SDK debug libs installed at {os.path.abspath(cls.vulkanDirectory)}")
        else:
            print(f"\nVulkan SDK debug libs located at {os.path.abspath(cls.vulkanDirectory)}")
        return True

if __name__ == "__main__":
    VulkanConfiguration.Validate()
