
import os
import sys
from platform import platform
import subprocess

import Utils
import SetupPackages
from SetupVulkan import VulkanConfiguration as VulkanRequirements
from SetupPython import PythonConfiguration
from SetupPremake import PremakeConfiguration as PremakeRequirements

def main(argv):

    if Utils.IsWindows():
        print("Hazard for Windows\n")
    elif Utils.IsMacos():
           print("Hazard for MacOS\n")
        
    #Init colorama
    if not SetupPackages.PackageInstalled('colorama'):
        SetupPackages.InstallPackage('colorama')

    import colorama
    from colorama import Fore
    from colorama import Back
    from colorama import Style

    colorama.init()
    os.chdir("../")
    
    
    #Check if 'requests' package exists
    print("Checking packages")
    requestsInstalled = SetupPackages.PackageInstalled('requests')

    if not requestsInstalled:
        SetupPackages.PromptInstall('packages')

    #Check premake settings
    print("\nValidating premake")
    premakeInstalled = PremakeRequirements.Validate('5.0.0-beta1')

    if not premakeInstalled:
        print(f"{Style.BRIGHT}{Back.YELLOW}Premake is not installed{Style.RESET_ALL}")
        if Utils.YesNo("Install Premake? [Y/N]"):
            PremakeRequirements.InstallPremake();
            print(f"{Style.BRIGHT}{Back.GREEN}Premake installed{Style.RESET_ALL}")
            requestsInstalled = True

    #Check vulkan version
    if Utils.IsWindows():
        print("\nValidating Vulkan version")
        vulkanInstalled = VulkanRequirements.Validate("1.3.216.0")
        
        if not vulkanInstalled:
            if Utils.YesNo("Vulkan SDK not found, would you like to install it [Y/N]"):
                VulkanRequirements.InstallVulkanSDK()
        else:
            print(f"{Style.BRIGHT}{Back.GREEN}Vulkan SDK located{Style.RESET_ALL}")

    print("Current branch branch")
    subprocess.call(["git", "branch"])
    print("Updating submodules")

    #subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

    print("\nRunning premake")
    
    if Utils.IsWindows():
        subprocess.check_call(["vendor/premake/bin/premake5.exe", argv[0]])
        
    elif Utils.IsMacos():
        platformOverride = ""
        if len(argv) > 0:
            platformOverride = "--os=" + argv[0]
            
        subprocess.check_call(["vendor/premake/bin/premake5", platformOverride, "xcode4"])

    print("\nSetup completed")

if __name__ == "__main__":
   main(sys.argv[1:])
