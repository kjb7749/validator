#include <iostream>
#include <fstream>
#include <re2/re2.h>
#include <Windows.h>
#include <vector>

using namespace std;

DWORD convert_ansi_to_unicode_string(
    __out std::wstring& unicode,
    __in const char* ansi,
    __in const size_t ansi_size
) {
    DWORD error = 0;
    do {
        if ((nullptr == ansi) || (0 == ansi_size)) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }
        unicode.clear();
        //
        // getting required cch.
        //
        int required_cch = ::MultiByteToWideChar(
            CP_ACP,
            0,
            ansi, static_cast<int>(ansi_size),
            nullptr, 0
        );
        if (0 == required_cch) {
            error = ::GetLastError();
            break;
        }
        unicode.resize(required_cch);
        //
        // convert.
        //
        if (0 == ::MultiByteToWideChar(
            CP_ACP,
            0,
            ansi, static_cast<int>(ansi_size),
            const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
        )) {
            error = ::GetLastError();
            break;
        }
    } while (false);
    return error;
}

DWORD convert_unicode_to_ansi_string(
    __out std::string& ansi,
    __in const wchar_t* unicode,
    __in const size_t unicode_size
) {
    DWORD error = 0;
    do {
        if ((nullptr == unicode) || (0 == unicode_size)) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }
        ansi.clear();
        //
        // getting required cch.
        //
        int required_cch = ::WideCharToMultiByte(
            CP_ACP,
            0,
            unicode, static_cast<int>(unicode_size),
            nullptr, 0,
            nullptr, nullptr
        );
        if (0 == required_cch) {
            error = ::GetLastError();
            break;
        }
        //
        // allocate.
        //
        ansi.resize(required_cch);
        //
        // convert.
        //
        if (0 == ::WideCharToMultiByte(
            CP_ACP,
            0,
            unicode, static_cast<int>(unicode_size),
            const_cast<char*>(ansi.c_str()), static_cast<int>(ansi.size()),
            nullptr, nullptr
        )) {
            error = ::GetLastError();
            break;
        }
    } while (false);
    return error;
}

DWORD convert_utf8_to_unicode_string(
    __out std::wstring& unicode,
    __in const char* utf8,
    __in const size_t utf8_size
) {
    DWORD error = 0;
    do {
        if ((nullptr == utf8) || (0 == utf8_size)) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }
        unicode.clear();
        //
        // getting required cch.
        //
        int required_cch = ::MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            utf8, static_cast<int>(utf8_size),
            nullptr, 0
        );
        if (0 == required_cch) {
            error = ::GetLastError();
            break;
        }
        //
        // allocate.
        //
        unicode.resize(required_cch);
        //
        // convert.
        //
        if (0 == ::MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            utf8, static_cast<int>(utf8_size),
            const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
        )) {
            error = ::GetLastError();
            break;
        }
    } while (false);
    return error;
}


DWORD convert_unicode_to_utf8_string(
    __out std::string& utf8,
    __in const wchar_t* unicode,
    __in const size_t unicode_size
) {
    DWORD error = 0;
    do {
        if ((nullptr == unicode) || (0 == unicode_size)) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }
        utf8.clear();
        //
        // getting required cch.
        //
        int required_cch = ::WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            unicode, static_cast<int>(unicode_size),
            nullptr, 0,
            nullptr, nullptr
        );
        if (0 == required_cch) {
            error = ::GetLastError();
            break;
        }
        //
        // allocate.
        //
        utf8.resize(required_cch);
        //
        // convert.
        //
        if (0 == ::WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            unicode, static_cast<int>(unicode_size),
            const_cast<char*>(utf8.c_str()), static_cast<int>(utf8.size()),
            nullptr, nullptr
        )) {
            error = ::GetLastError();
            break;
        }
    } while (false);
    return error;
}


auto fileReader = [](string filePath)
{
    std::ifstream in(filePath);
    std::string s;

    if (in.is_open()) {
        // 위치 지정자를 파일 끝으로 옮긴다.
        in.seekg(0, std::ios::end);

        // 그리고 그 위치를 읽는다. (파일의 크기)
        int size = in.tellg();

        // 그 크기의 문자열을 할당한다.
        s.resize(size);

        // 위치 지정자를 다시 파일 맨 앞으로 옮긴다.
        in.seekg(0, std::ios::beg);

        // 파일 전체 내용을 읽어서 문자열에 저장한다.
        in.read(&s[0], size);

        return s;
    }
    else {
        std::cout << "파일을 찾을 수 없습니다!" << std::endl;
    }
};


auto splitString = [](wstring text, wstring spliter) -> vector<wstring>
{
    vector<wstring> substrings;
    size_t pos = 0;
    wstring token;
    while ((pos = text.find(spliter)) != wstring::npos)
    {
        token = text.substr(0, pos);
        substrings.push_back(token);
        text.erase(0, pos + spliter.length());
    }
    substrings.push_back(text);

    return substrings;
};

auto trimString = [](wstring text, wstring base, wstring target) -> wstring
{
    size_t index;
    size_t length = lstrlen(base.c_str());

    if (length <= 0)
        return text;

    while ((index = text.find(base)) != wstring::npos)
    {
        text.replace(index, length, target);
    }
    return text;
};


auto normalizer = [](wstring text) -> vector<wstring>
{
    text = trimString(text, TEXT("\t"), TEXT(""));
    text = trimString(text, TEXT("\n"), TEXT(""));
    //text = trimString(text, TEXT("/"), TEXT(""));
    text = trimString(text, TEXT(" "), TEXT(""));
    vector<wstring> strings = splitString(text, TEXT(","));
    std::sort(strings.begin(), strings.end());
    return strings;
};

int main()
{   
    string utf8Text;
    wstring unicodeBaseText;
    wstring unicodeTargetText;
    string pattern;


    utf8Text = fileReader("testHeader.h");          //utf8 형태로 인코딩 된 파일에서 데이터를 읽는다
    RE2::Replace(&utf8Text, "//.*", "");

    convert_utf8_to_unicode_string(unicodeBaseText, utf8Text.c_str(), strlen(utf8Text.c_str()));        //unicode 문자열로 변환한다.

    utf8Text.clear();
    utf8Text = fileReader("testHeader2.h");
    RE2::Replace(&utf8Text, "//", "");

    convert_utf8_to_unicode_string(unicodeTargetText, utf8Text.c_str(), strlen(utf8Text.c_str()));        //unicode 문자열로 변환한다.

    auto baseStrs = normalizer(unicodeBaseText);
    auto targetStrs = normalizer(unicodeTargetText);



    for (auto itr : baseStrs)
    {
        string token;
        convert_unicode_to_ansi_string(token, itr.c_str(), lstrlen(itr.c_str()));
        cout << token << endl;
    }


    if (baseStrs.size() != targetStrs.size())
    {
        MessageBox(0, TEXT("개수가 일치하지 않습니다."), TEXT("개수 불일치"), 0);
    }

    size_t size = min(baseStrs.size(), targetStrs.size());

    for(size_t index = 0; index < size; ++index)
    {
        if (baseStrs.at(index) != targetStrs.at(index))
        {
            MessageBox(0, TEXT("데이터가 밀렸습니다. 확인 부탁드려요!"), TEXT("데이터 밀림"), 0);
        }
    }

    return 0;
}