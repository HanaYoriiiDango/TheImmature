// StringUtils.cpp
#include "StringUtils.h"
#include <stringapiset.h> // Для WideCharToMultiByte и MultiByteToWideChar

std::wstring StringUtils::UTF8ToWide(const std::string& utf8Str) {
    if (utf8Str.empty()) return L"";

    int wideSize = MultiByteToWideChar(CP_UTF8, 0,
        utf8Str.c_str(), -1,
        nullptr, 0);
    if (wideSize == 0) return L"";

    std::wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0,
        utf8Str.c_str(), -1,
        &wideStr[0], wideSize);

    // Убираем нулевой символ в конце
    wideStr.pop_back();
    return wideStr;
}

std::string StringUtils::WideToUTF8(const std::wstring& wideStr) {
    if (wideStr.empty()) return "";

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0,
        wideStr.c_str(), -1,
        nullptr, 0,
        nullptr, nullptr);
    if (utf8Size == 0) return "";

    std::string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0,
        wideStr.c_str(), -1,
        &utf8Str[0], utf8Size,
        nullptr, nullptr);

    utf8Str.pop_back();
    return utf8Str;
}