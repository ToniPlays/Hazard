#ifdef HZR_PLATFORM_IOS

#include "UtilityCore.h"
#include "File.h"
#include "Utility/StringUtil.h"

#include "spdlog/fmt/fmt.h"
#include <Directory.h>

#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>

std::string File::OpenFileDialog()
{
    return "";
}

std::string File::OpenFileDialog(const std::vector<std::string>& filters)
{
    return "";
}
std::string File::SaveFile(const std::vector<std::string>& filters, const std::filesystem::path& defaultPath)
{
    return "";
}

bool File::Exists(const std::filesystem::path& path)
{
    std::string filePath = path.parent_path() / GetNameNoExt(path);
    std::string extension = GetFileExtension(path);
    
    NSString* fileName = [NSString stringWithUTF8String: filePath.c_str()];
    
    NSBundle* bundle = [NSBundle mainBundle];
    return [bundle pathForResource: fileName ofType: [NSString stringWithUTF8String: extension.c_str()]];
    
}
bool File::IsDirectory(const std::filesystem::path& path)
{
    return std::filesystem::is_directory(path);
}

std::filesystem::path File::FindAvailableName(const std::filesystem::path& directory, const std::string& name, const std::string& extension)
{
    return "";
}

bool File::WriteBinaryFile(const std::filesystem::path& path, void* data, uint64_t size)
{
    return false;
}
bool File::WriteFile(const std::filesystem::path& file, const std::string& content)
{
    return true;
}
bool File::CopyFileTo(const std::filesystem::path& source, const std::filesystem::path& dest)
{
    return false;
}
bool File::IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo)
{
    return false;
}


bool File::Move(const std::filesystem::path& src, const std::filesystem::path& dst)
{
    return false;
}

std::string File::ReadFile(const std::filesystem::path& file)
{
    @autoreleasepool {
        
        
        std::string filePath = file.parent_path() / GetNameNoExt(file);
        std::string extension = GetFileExtension(file);
        
        NSString* fileName = [NSString stringWithUTF8String: filePath.c_str()];
        
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* path = [bundle pathForResource: fileName ofType: [NSString stringWithUTF8String: extension.c_str()]];
        
        if(!path)
            return "";
        
        NSString* content = [NSString stringWithContentsOfFile: path encoding: kCFStringEncodingUTF8 error: nil];
        
        return content.UTF8String;
    }
}
CachedBuffer File::ReadBinaryFile(const std::filesystem::path& path)
{
    @autoreleasepool {
        
        
        std::string filePath = path.parent_path() / GetNameNoExt(path);
        std::string extension = GetFileExtension(path);
        
        NSString* fileName = [NSString stringWithUTF8String: filePath.c_str()];
        
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* bundlePath = [bundle pathForResource: fileName ofType: [NSString stringWithUTF8String: extension.c_str()]];
        
        if(!bundlePath)
            return CachedBuffer();
        
        NSData* content = [NSData dataWithContentsOfFile: bundlePath];
        CachedBuffer result(content.length);
        result.Write(content.bytes, content.length);
        return result;
    }
}

std::filesystem::path File::GetFileAbsolutePath(const std::filesystem::path& file)
{
    return file;
}
std::filesystem::path File::GetDirectoryOf(const std::filesystem::path& file)
{
    return "";
}
std::string File::GetName(const std::filesystem::path& file)
{
    return file.filename().string();
}
std::string File::GetNameNoExt(const std::filesystem::path& file)
{
    auto name = GetName(file);
    return name.substr(0, name.find_last_of('.'));
}
std::filesystem::path File::GetPathNoExt(const std::filesystem::path& file)
{
    return "";
}
std::string File::GetFileExtension(const std::filesystem::path& file)
{
    return file.extension().string();
}

void File::Copy(const std::filesystem::path& source, const std::filesystem::path& dest, CopyOptions options)
{
    
}
#endif
