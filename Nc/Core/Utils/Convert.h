
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


#ifndef NC_CORE_UTILS_CONVERT_H_
#define NC_CORE_UTILS_CONVERT_H_

#include <sstream>
#include <math.h>
#include <cctype>    // for tolower and toupper
#include <algorithm> // for transform

#include "../Define.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846 ///< define M_PI if it is not defined
#endif

namespace Nc
{
    namespace Utils
    {
        /// this namespace provide some Fonctor class utility
        namespace Fonctor
        {
            struct ToLower  {char operator()(char c) const  {return std::tolower(static_cast<unsigned char>(c));} };
            struct ToUpper  {char operator()(char c) const  {return std::toupper(static_cast<unsigned char>(c));} };
        }

        /// this namespace provide some function to convert data
        namespace Convert
        {
            /// Convert Degree to Radians number
            inline double DegToRad(double deg)           {return (deg*M_PI)/180.0;}
            /// Convert Radians to Degree number
            inline double RadToDeg(double rad)           {return (rad*180.0)/M_PI;}

            /// Convert an std::string to the type T
            /** Fill the data passed in parameter, and return true if the translation succeed */
            template <typename T>
            bool StringTo(const std::string &s, T &dest);

            /// Convert a data T to an std::string
            template <typename T>
            std::string ToString(const T &data);

            /// Convert the first occurence of the delimitor to T in dest
            template <typename T>
            bool SplitStringTo(std::string &str, const std::string &delimitor, T &dest);

            /// Convert the caracters of the string in lower caracters
            inline void ToLower(std::string &s)     {std::transform(s.begin(), s.end(), s.begin(), Fonctor::ToLower());}
            /// Convert the caracters of the string in upper caracters
            inline void ToUpper(std::string &s)     {std::transform(s.begin(), s.end(), s.begin(), Fonctor::ToUpper());}


            namespace Unicode
            {
                /** Generic function to convert an UTF-32 characters range
                    to an ANSI characters range, using the given locale class

                    \param begin :  Iterator pointing to the beginning of the input sequence
                    \param end :    Iterator pointing to the end of the input sequence
                    \param output : Iterator pointing to the beginning of the output sequence
                    \param lc :     Locale to use for conversion
                    \return Iterator to the end of the output sequence which has been written
                */
                template<typename In, typename Out>
                Out UTF32ToANSI(In begin, In end, Out output, char replacement = '?', const std::locale& lc = std::locale());

                /** Generic function to convert an ANSI characters range
                    to an UTF-32 characters range, using the given locale class

                    \param begin :  Iterator pointing to the beginning of the input sequence
                    \param end :    Iterator pointing to the end of the input sequence
                    \param output : Iterator pointing to the beginning of the output sequence
                    \param lc :     Locale to use for conversion
                    \return Iterator to the end of the output sequence which has been written
                */
                template<typename In, typename Out>
                Out ANSIToUTF32(In begin, In end, Out output, const std::locale& lc = std::locale());

                /** Generic function to convert an UTF-16 characters range
                    to an UTF-32 characters range, using the given locale class

                    \param begin :  Iterator pointing to the beginning of the input sequence
                    \param end :    Iterator pointing to the end of the input sequence
                    \param output : Iterator pointing to the beginning of the output sequence
                    \param lc :     Locale to use for conversion
                    \return Iterator to the end of the output sequence which has been written
                */
                template <typename In, typename Out>
                Out UTF16ToUTF32(In begin, In end, Out output, Nc::UInt32 replacement = '?');

                /** Generic function to convert an UTF-32 characters range
                    to an UTF-16 characters range, using the given locale class

                    \param begin :  Iterator pointing to the beginning of the input sequence
                    \param end :    Iterator pointing to the end of the input sequence
                    \param output : Iterator pointing to the beginning of the output sequence
                    \param lc :     Locale to use for conversion
                    \return Iterator to the end of the output sequence which has been written
                */
                template<typename In, typename Out>
                Out UTF32ToUTF16(In begin, In end, Out output, Nc::UInt16 replacement = '?');
            }

            #include "Convert.tpl"      // the template source code

        }
    }
}

#endif
