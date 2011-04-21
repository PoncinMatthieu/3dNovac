
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

/** \file
    include the principal files of the lib Nc-Utils
*/

#ifndef NC_CORE_UTILS_UTILS_H_
#define NC_CORE_UTILS_UTILS_H_

    #include "../Define.h"
    #include "Logger.h"
    #include "FileName.h"
    #include "Exception.h"
    #include "Convert.h"
    #include "SmartPointer.h"
    #include "Clock.h"
    #include "NonCopyable.h"
    #include "Singleton.h"
    #include "Unicode.h"

    namespace Nc
    {
        /// This namespace provide many usefull classes and function, like Logger, FileName and Singleton
        namespace Utils
        {
        // usefull functions
            /** template Strlen to use with unicode string for exemple*/
            template<typename T>
            std::size_t Strlen(const T *str)
            {
                std::size_t len = 0;
                while (*str++)
                    len++;
                return len;
            }

            /** Delete a container like std::list */
            template<typename T>
            void    DeleteContainer(T &container)
            {
                while(!container.empty())
                {
                    delete (*container.begin());
                    container.pop_front();
                }
            }

        // usefull templates
            /// conditional inheritance
            template<bool T, typename TRUE_CLASS, typename FALSE_CLASS>
            struct IF : TRUE_CLASS {};

            /// specialisation of the struct IF, to make the inheritance with the conditional to false
            template<typename TRUE_CLASS, typename FALSE_CLASS>
            struct IF<false, TRUE_CLASS, FALSE_CLASS> : FALSE_CLASS {};

            /// container of type T
            template<typename T>
            struct Container    { T   Data; };
        }
    }


#endif
