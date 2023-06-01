
import importlib
import sys
import subprocess
import Utils

def PackageInstalled(package):
    if package in sys.modules:
        return True
    return importlib.util.find_spec(package) is not None

def InstallPackage(package):
    if Utils.IsWindows():
        subprocess.check_call(["pip", "install", package], stdout = subprocess.DEVNULL)
    elif Utils.IsMacos():
        subprocess.check_call(['pip3', 'install', package], stdout = subprocess.DEVNULL)
    
def PromptInstall(package):
    print(f"{Style.BRIGHT}{Back.YELLOW}Python package '{package}' not found{Style.RESET_ALL}")
    if Utils.YesNo("Install package? [Y/N]"):
        SetupPackages.InstallPackage('requests')
        print(f"{Style.BRIGHT}{Back.GREEN}Package '{package}' installed{Style.RESET_ALL}")
        return true
    return false
