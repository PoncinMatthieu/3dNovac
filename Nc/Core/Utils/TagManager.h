
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

    File Created At:        10/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_TAGMANAGER_H_
#define NC_CORE_UTILS_TAGMANAGER_H_

#include "../Define.h"

namespace Nc
{
    namespace Utils
    {
        /// Define a class to manage a Tag system to associate a unique tag to a class for the given VTableType
        template<typename VTableType>
        struct TagManager
        {
            private:
            /// Define a tag counter to put a unique tag on each visitable class
                template<typename Base>
                struct TagCounter
                {
                    static size_t       counter; // default 0
                };

            /// Define a class witch store the tag for each Visitable associated to the given Base
                template<typename Visitable, typename Base>
                struct TagHolder
                {
                    static size_t       tag;
                };

            public:
                /** \return the tag of a visitable */
                template<typename Visitable, typename Base>
                static size_t GetTag()
                {
                    size_t &tag = TagHolder<const Visitable, const Base>::tag;
                    // first time : generate tag
                    if (tag == 0)
                    {
                        tag = ++TagCounter<const Base>::counter;
                        //LOG << "Generate tag " << tag << "  with VtableType: " << typeid(VTableType).name() << " and base: " << typeid(Base).name() << "     for " << typeid(Visitable).name() << std::endl;
                    }
                    return tag;
                }
        };

        template<typename VTableType>
        template<typename Base>
        size_t TagManager<VTableType>::TagCounter<Base>::counter = 0;

        template<typename VTableType>
        template<typename Visitable, typename Base>
        size_t TagManager<VTableType>::TagHolder<Visitable, Base>::tag = TagManager<VTableType>::GetTag<Visitable, Base>();
    }
}

#endif