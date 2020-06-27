#include <iostream>
#include <fstream>
#include <re2/re2.h>
#include <Windows.h>

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


int main()
{
    wstring a = TEXT("한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글");
    wstring b = TEXT("한글");

    //string a = "한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글";
    //string b = "한글";

    string text;
    string pattern;

    convert_unicode_to_utf8_string(text, a.c_str(), lstrlenW(a.c_str()));
    convert_unicode_to_utf8_string(pattern, b.c_str(), lstrlenW(b.c_str()));

    cout << RE2::PartialMatch(text, pattern) <<endl;


    auto s = fileReader("test.txt");

    wstring unicodeText;
    string utf8Text;

    //convert_ansi_to_unicode_string(unicodeText, s.c_str(), s.length());
    //convert_unicode_to_utf8_string(utf8Text, unicodeText.c_str(), unicodeText.length());

    cout << RE2::PartialMatch(s, pattern) << endl;


    //MessageBox(0, TEXT("내용"), TEXT("제목"), 0);
}