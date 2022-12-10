
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
    