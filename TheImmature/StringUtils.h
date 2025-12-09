// StringUtils.h 
#pragma once
#include <windows.h>
#include <string>

namespace StringUtils {
    // UTF-8 (из JSON) → UTF-16 (wstring для Windows)
    std::wstring UTF8ToWide(const std::string& utf8Str);

    // UTF-16 (wstring) → UTF-8 (для JSON)
    std::string WideToUTF8(const std::wstring& wideStr);

    // Проверка на пустую строку
    bool IsEmpty(const std::wstring& str);
    bool IsEmpty(const std::string& str);
}
