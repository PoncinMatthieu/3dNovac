
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        13/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_METAPROG_H_
#define NC_CORE_UTILS_METAPROG_H_

#include "../Define.h"

namespace Nc
{
    namespace Utils
    {
        /// Define some usefull metaprogramming templates
        namespace Metaprog
        {
            /// conditional inheritance
            template<bool T, typename TRUE_CLASS, typename FALSE_CLASS>
            struct If : TRUE_CLASS {};

            /// specialisation of the struct IF, to make the inheritance with the conditional to false
            template<typename TRUE_CLASS, typename FALSE_CLASS>
            struct If<false, TRUE_CLASS, FALSE_CLASS> : FALSE_CLASS {};

            /// container of type T
            template<typename T>
            struct Container    { T   Data; };

            struct Nop;
        }
    }
}

#endif // NC_CORE_UTILS_METAPROG_H_
