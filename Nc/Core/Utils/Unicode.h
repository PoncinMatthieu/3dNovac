
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

#ifndef NC_CORE_UTILS_UNICODE_H_
#define NC_CORE_UTILS_UNICODE_H_

#include <sstream>
#include <string>
#include "../Define.h"
#include "Convert.h"

namespace Nc
{
    namespace Utils
    {
        /// This namespace provide classes to manipulate any Unicode strings.
        namespace Unicode
        {
            /// Manipulate UTF32 string.
            class LIB_NC_CORE UTF32 : public std::basic_string<Nc::UInt32>
            {
                public:
                    UTF32()  {}
                    UTF32(const char *str);
                    UTF32(const wchar_t *str);
                    UTF32(const Nc::UInt32 *str);
                    UTF32(const std::string &str);
                    UTF32(const std::wstring &str);
                    virtual ~UTF32()    {}

                    UTF32 operator + (const UTF32 &str) const;

                    /** \return the number of occurence of the caractere 'c' in the unicode string. */
                    unsigned int        CharCount(Nc::UInt32 c) const;

                    /** Convert and return the unicode string in standard std::string */
                    std::string         ToStdString() const;
                    /** Convert and return the unicode string in standard std::string into the given std::string. */
                    std::string         &ToStdString(std::string &str) const;

                    /** Convert and return the unicode string in standard std::wstring */
                    std::wstring        ToStdWString() const;
                    /** Convert and return the unicode string in standard std::wstring into the given std::wstring. */
                    std::wstring        &ToStdWString(std::wstring &str) const;

                    friend LIB_NC_CORE std::ostream &operator << (std::ostream& os, const UTF32& s);
            };
        }
    }
}

#endif
