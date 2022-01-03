
import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

print(platform.system())
    
# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

if platform.system() == "Windows":
    from SetupVulkan import VulkanConfiguration as VulkanRequirements
    
    VulkanRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements

os.chdir('./../') # Change from devtools/scripts directory to root
    
premakeInstalled = PremakeRequirements.Validate()
print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning Premake...")
        subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"), "nopause"])
    elif platform.system() == "Darwin":
        print("\Running Premake...")
        subprocess.call(["sudo", os.path.abspath("./scripts/Mac-GenProjects.sh"), "nopause"]);
        
    print("\nSetup completed!")
else:
    print("Hazard requires Premake to generate project files.")
