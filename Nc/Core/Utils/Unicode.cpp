
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <iterator>
#include <iostream>
#include "Unicode.h"
#include "Utils.h"

using namespace std;
using namespace Nc::Utils;
using namespace Nc::Utils::Unicode;

UTF32::UTF32(const char *str)
{
    if (str)
    {
        size_t len = Strlen(str);
        if (len > 0)
        {
            reserve(len + 1);
            Convert::Unicode::ANSIToUTF32(str, str + len, std::back_inserter(*this));
        }
    }
}

UTF32::UTF32(const wchar_t *str)
{
    // This function assumes that 2-byte large wchar_t are encoded in UTF-16 (Windows), and
    // 4-byte large wchar_t are encoded using UTF-32 (Unix)
    // Is that always true ? (some platforms may use JIS Japanese encoding)
    // The macro __STDC_ISO_10646__ should help identifying UTF-32 compliant implementations

    if (str)
    {
        size_t len = Strlen(str);
        if (len > 0)
        {
            reserve(len + 1);
            switch (sizeof(wchar_t))
            {
                case 2 : Convert::Unicode::UTF16ToUTF32(str, str + len, std::back_inserter(*this), 0);  break;
                case 4 : std::copy(str, str + len, std::back_inserter(*this));                          break;
                default : break;
            }
        }
    }
}

UTF32::UTF32(const Nc::UInt32 *str)
{
    if (str)
        assign(str);
}

UTF32::UTF32(const std::string &str)
{
    size_t len = str.size();
    if (len > 0)
    {
        reserve(len + 1);
        Convert::Unicode::ANSIToUTF32(str.begin(), str.end(), std::back_inserter(*this));
    }
}

UTF32::UTF32(const std::wstring &str)
{
    // This function assumes that 2-byte large wchar_t are encoded in UTF-16 (Windows), and
    // 4-byte large wchar_t are encoded using UTF-32 (Unix)
    // Is that always true ? (some platforms may use JIS Japanese encoding)
    // The macro __STDC_ISO_10646__ should help identifying UTF-32 compliant implementations

    size_t len = str.size();
    if (len > 0)
    {
        reserve(len + 1);

        // Select the proper function according to the (supposed) wchar_t system encoding
        switch (sizeof(wchar_t))
        {
            case 2: Convert::Unicode::UTF16ToUTF32(str.begin(), str.end(), std::back_inserter(*this), 0);   break;
            case 4: std::copy(str.begin(), str.end(), std::back_inserter(*this));                           break;
            default : break;
        }
    }
}

UTF32::UTF32(const std::basic_string<Nc::UInt32> &str)
    : std::basic_string<Nc::UInt32>(str)
{

}

UTF32 UTF32::operator + (const UTF32 &str) const
{
    UTF32 newStr(*this);
    newStr.append(str);
    return newStr;
}

std::string             UTF32::ToStdString() const
{
    std::string str;
    return ToStdString(str);
}

std::string             &UTF32::ToStdString(std::string &str) const
{
    str.reserve(size() + 1);
    Convert::Unicode::UTF32ToANSI(begin(), end(), std::back_inserter(str), 0);
    return str;
}

std::wstring            UTF32::ToStdWString() const
{
    std::wstring str;
    return ToStdWString(str);
}

std::wstring            &UTF32::ToStdWString(std::wstring &str) const
{
    // This function assumes that 2-byte large wchar_t are encoded in UTF-16 (Windows), and
    // 4-byte large wchar_t are encoded using UTF-32 (Unix)
    // Is that always true ? (some platforms may use JIS Japanese encoding)
    // The macro __STDC_ISO_10646__ should help identifying UTF-32 compliant implementations

    str.reserve(size() + 1);

    // Select the proper function according to the (supposed) wchar_t system encoding
    switch (sizeof(wchar_t))
    {
        case 2 : Convert::Unicode::UTF32ToUTF16(begin(), end(), std::back_inserter(str), 0);      break;
        case 4 : std::copy(begin(), end(), std::back_inserter(str));                              break;
        default : break;
    }
    return str;
}

unsigned int    UTF32::CharCount(Nc::UInt32 c) const
{
    unsigned int r = 0;
    const Nc::UInt32 *p = c_str();
    for (unsigned int i = 0; i < size(); ++i)
    {
        if (p[i] == c)
            r++;
    }
    return r;
}

namespace Nc
{
	namespace Utils
	{
		namespace Unicode
		{
			std::ostream &operator << (std::ostream& os, const UTF32& s)
			{
				os << s.ToStdString();
				return os;
			}
		}
	}
}
