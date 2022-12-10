
import platform;

def IsWindows():
    return platform.system() == "Windows"
def IsMacos():
    return platform.system() == "Darwin"

def PlatformExecutable():
    if IsWindows():
        return "exe"
    elif IsMacos():
        return "dmg"
    else:
        return ""

def YesNo(question):
    return str(input(question)).lower().strip()[:1] == "y"

def GetSystemEnvironmentVariable(name):
    key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, r"System\CurrentControlSet\Control\Session Manager\Environment")
    try:
        return winreg.QueryValueEx(key, name)[0]
    except:
        return None

def GetUserEnvironmentVariable(name):
    key = winreg.CreateKey(winreg.HKEY_CURRENT_USER, r"Environment")
    try:
        return winreg.QueryValueEx(key, name)[0]
    except:
        return None
